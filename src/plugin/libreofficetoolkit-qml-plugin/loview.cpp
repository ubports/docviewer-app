/*
 * Copyright (C) 2015 Canonical, Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "loview.h"
#include "lodocument.h"
#include "sgtileitem.h"
#include "twips.h"
#include "config.h"

#include <QImage>
#include <QTimer>
#include <QtCore/qmath.h>

static qreal zoomValueToFitWidth;

static qreal getZoomToFitWidth(const qreal &width, int documentWidth)
{
    return qreal(width / Twips::convertTwipsToPixels(documentWidth, 1.0));
}

LOView::LOView(QQuickItem *parent)
    : QQuickItem(parent)
    , m_parentFlickable(nullptr)
    , m_document(nullptr)
    , m_partsModel(nullptr)
    , m_zoomFactor(1.0)
    , m_cacheBuffer(TILE_SIZE * 3)
    , m_visibleArea(0, 0, 0, 0)
    , m_bufferArea(0, 0, 0, 0)
    , m_error(LibreOfficeError::NoError)
{
    Q_UNUSED(parent)   

    connect(this, SIGNAL(documentChanged()), this, SLOT(updateViewSize()));
    connect(this, SIGNAL(zoomFactorChanged()), this, SLOT(updateViewSize()));
    connect(this, SIGNAL(parentFlickableChanged()), this, SLOT(updateVisibleRect()));
    connect(this, SIGNAL(cacheBufferChanged()), this, SLOT(updateVisibleRect()));
    connect(&m_updateTimer, SIGNAL(timeout()), this, SLOT(updateVisibleRect()));

    connect(RenderEngine::instance(), &RenderEngine::taskRenderFinished,
            this, &LOView::slotTaskRenderFinished);
}

// Returns the parent QML Flickable
QQuickItem* LOView::parentFlickable() const
{
    return m_parentFlickable;
}

// Set the parent QML Flickable
void LOView::setParentFlickable(QQuickItem *flickable)
{
    if (m_parentFlickable == flickable)
        return;

    if (m_parentFlickable)
        m_parentFlickable->disconnect(this);

    m_parentFlickable = flickable;

    connect(m_parentFlickable, SIGNAL(widthChanged()), this, SLOT(updateVisibleRect()));
    connect(m_parentFlickable, SIGNAL(heightChanged()), this, SLOT(updateVisibleRect()));

    connect(m_parentFlickable, SIGNAL(contentXChanged()), this, SLOT(scheduleVisibleRectUpdate()));
    connect(m_parentFlickable, SIGNAL(contentYChanged()), this, SLOT(scheduleVisibleRectUpdate()));

    Q_EMIT parentFlickableChanged();
}

void LOView::initializeDocument(const QString &path)
{
    if (m_document)
        m_document->disconnect(this);

    setError(LibreOfficeError::NoError);

    m_document = QSharedPointer<LODocument>(new LODocument());
    m_document->setPath(path);

    /* A lot of things happens when we set the path property in
     * m_document. Need to check if an error has been emitted. */
    if (m_document->error() != LibreOfficeError::NoError) {
        setError(m_document->error());

        m_document.clear();

        // Stop doing anything below.
        return;
    }

    // TODO MOVE
    m_partsModel = new LOPartsModel(m_document);
    Q_EMIT partsModelChanged();

    // --------------------------------------------------
    QQmlEngine *engine = QQmlEngine::contextForObject(this)->engine();
    if (engine->imageProvider("lok"))
        engine->removeImageProvider("lok");

    m_imageProvider = new LOPartsImageProvider(m_document);
    engine->addImageProvider("lok", m_imageProvider);
    // --------------------------------------------------

    connect(m_document.data(), SIGNAL(currentPartChanged()), this, SLOT(invalidateAllTiles()));

    Q_EMIT documentChanged();
}

// Return the LODocument rendered by this class
LODocument* LOView::document() const
{
    return m_document.data();
}

LOPartsModel *LOView::partsModel() const
{
    return m_partsModel;
}

qreal LOView::zoomFactor() const
{
    return m_zoomFactor;
}

void LOView::setZoomFactor(const qreal zoom)
{
    if (m_zoomFactor == zoom)
        return;

    m_zoomFactor = zoom;

   if (m_zoomFactor != zoomValueToFitWidth)
        setZoomMode(LOView::Manual);

    Q_EMIT zoomFactorChanged();
}

LOView::ZoomMode LOView::zoomMode() const
{
    return m_zoomMode;
}

void LOView::setZoomMode(const ZoomMode zoomMode)
{
    if (m_zoomMode == zoomMode)
        return;

    m_zoomMode = zoomMode;
    Q_EMIT zoomModeChanged();
}

int LOView::cacheBuffer() const
{
    return m_cacheBuffer;
}

void LOView::setCacheBuffer(int cacheBuffer)
{
    if (m_cacheBuffer == cacheBuffer)
        return;

    m_cacheBuffer = cacheBuffer;
    Q_EMIT cacheBufferChanged();
}

LibreOfficeError::Error LOView::error() const
{
    return m_error;
}

void LOView::adjustZoomToWidth()
{
    if (!m_document)
        return;

    setZoomMode(LOView::FitToWidth);

    zoomValueToFitWidth = getZoomToFitWidth(m_parentFlickable->width(),
                                            m_document->documentSize().width());

    setZoomFactor(zoomValueToFitWidth);
    qDebug() << "Adjust zoom to width - value:" << zoomValueToFitWidth;
}

void LOView::updateViewSize()
{
    if (!m_document)
        return;

    QSize docSize = m_document->documentSize();

    this->setWidth(Twips::convertTwipsToPixels(docSize.width(), m_zoomFactor));
    this->setHeight(Twips::convertTwipsToPixels(docSize.height(), m_zoomFactor));

    updateVisibleRect();
}

void LOView::updateVisibleRect()
{
    if (!m_parentFlickable || !m_document)
        return;

    // Changes in parentFlickable width/height trigger directly LOView::updateVisibleRect(),
    // since they don't imply a change in the zoom factor - i.e. LOView::updateViewSize().
    // Anyway, this class also handle an automatic zoom when the parentFlickable has been
    // resized, so we need to take care of it.
    // updateZoomIfAutomatic() returns a bool, which is true when the zoomFactor is
    // set to a new value.
    // If that happens, stop the execution of this function, since the change of
    // zoomFactor will trigger the updateViewSize() function, which triggers this
    // function again.
    if (m_zoomMode == LOView::FitToWidth) {
        zoomValueToFitWidth = getZoomToFitWidth(m_parentFlickable->width(),
                                                m_document->documentSize().width());

        if (m_zoomFactor != zoomValueToFitWidth) {
            setZoomFactor(zoomValueToFitWidth);

            qDebug() << "Adjust automatic zoom to width - value:" << zoomValueToFitWidth;
            return;
        }
    }

    // Check if current tiles have a different zoom value
    if (!m_tiles.isEmpty()) {
        SGTileItem* tile = m_tiles.first();

        if (tile->zoomFactor() != m_zoomFactor) {
            clearView();

#ifdef DEBUG_VERBOSE
            qDebug() << "Zoom value of tiles is different than the current zoom value. Erasing cache...";
#endif
        }
    }

    // Just for convenience.
    QRect documentRect(this->boundingRect().toRect());

    // Update visible area
    QRect visibleRect(m_parentFlickable->property("contentX").toInt(),
                      m_parentFlickable->property("contentY").toInt(),
                      m_parentFlickable->width(),
                      m_parentFlickable->height());

    m_visibleArea = visibleRect.intersected(documentRect);

    // Update buffer area
    QRect bufferRect(m_visibleArea.left()   -  m_cacheBuffer,
                     m_visibleArea.top()    -  m_cacheBuffer,
                     m_visibleArea.width()  + (m_cacheBuffer * 2),
                     m_visibleArea.height() + (m_cacheBuffer * 2));

    m_bufferArea = bufferRect.intersected(documentRect);

    // Delete tiles that are outside the loading area
    if (!m_tiles.isEmpty()) {
        auto i = m_tiles.begin();
        while (i != m_tiles.end()) {
            SGTileItem* sgtile = i.value();

            // Ok - we still need this item.
            if (m_bufferArea.intersects(sgtile->area())) {
                i++;
                continue;
            }

            // Out of buffer - we should delete this item.
#ifdef DEBUG_VERBOSE
            qDebug() << "Removing tile indexed as" << i.key();
#endif

            RenderEngine::instance()->dequeueTask(sgtile->id());
            sgtile->deleteLater();
            i = m_tiles.erase(i);
        }
    }

    // Number of tiles per row
    int tilesPerWidth           = qCeil(this->width() / TILE_SIZE);
    int tilesPerHeight           = qCeil(this->height() / TILE_SIZE);

    // Get indexes for visible tiles
    int visiblesFromWidth       = int(m_visibleArea.left() / TILE_SIZE);
    int visiblesFromHeight      = int(m_visibleArea.top() / TILE_SIZE);
    int visiblesToWidth         = qCeil(qreal(m_visibleArea.right()) / TILE_SIZE);
    int visiblesToHeight        = qCeil(qreal(m_visibleArea.bottom()) / TILE_SIZE);

    // Get indexes for tiles in the visible area
    int bufferFromWidth         = int(m_bufferArea.left() / TILE_SIZE);
    int bufferFromHeight        = int(m_bufferArea.top() / TILE_SIZE);
    int bufferToWidth           = qCeil(qreal(m_bufferArea.right()) / TILE_SIZE);
    int bufferToHeight          = qCeil(qreal(m_bufferArea.bottom()) / TILE_SIZE);

#ifdef DEBUG_VERBOSE
    qDebug() << "Visible area - Left:" << visiblesFromWidth << "Right:" << visiblesToWidth << "Top:" << visiblesFromHeight << "Bottom:" << visiblesToHeight;
    qDebug() << "Buffer area  - Left:" << bufferFromWidth   << "Right:" << bufferToWidth   << "Top:" << bufferFromHeight   << "Bottom:" << bufferToHeight;
#endif

    this->generateTiles(visiblesFromWidth, visiblesFromHeight, visiblesToWidth, visiblesToHeight, tilesPerWidth, tilesPerHeight);
    this->generateTiles(bufferFromWidth,   bufferFromHeight,   bufferToWidth,   bufferToHeight,   tilesPerWidth, tilesPerHeight);
}

void LOView::generateTiles(int x1, int y1, int x2, int y2, int tilesPerWidth, int tilesPerHeight)
{
    for (int x = x1; x < x2; x++) {
        for (int y = y1; y < y2; y++) {
            bool lastRow    = (y == (tilesPerHeight - 1));
            bool lastColumn = (x == (tilesPerWidth  - 1));

            int left   = TILE_SIZE * x;
            int top    = TILE_SIZE * y;
            int width  = lastColumn ? this->width() - left : TILE_SIZE;
            int height = lastRow    ? this->height() - top : TILE_SIZE;

            QRect tileRect(left, top, width, height);
            int index = x + tilesPerWidth * y;

            createTile(index, tileRect);

#ifdef DEBUG_VERBOSE
            qDebug() << "Generating tile - Index:" << index << "X:" << x << "Y:" << y;
#endif
        }
    }
}

void LOView::invalidateAllTiles()
{
    clearView();
    updateViewSize();
}

void LOView::slotTaskRenderFinished(AbstractRenderTask* task, QImage img)
{
    if (task->type() == RttTile) {
        updateTileData(task, img);
    } else if (task->type() == RttImpressThumbnail) {
        updateThumbnailModel(task, img);
    }
}

void LOView::createTile(int index, QRect rect)
{
    if (!m_tiles.contains(index)) {
#ifdef DEBUG_VERBOSE
        qDebug() << "Creating tile indexed as" << index << "- Rect:" << rect;
#endif

        auto tile = new SGTileItem(rect, m_zoomFactor, RenderEngine::getNextId(), this);
        m_tiles.insert(index, tile);
        RenderEngine::instance()->enqueueTileTask(m_document, m_document->currentPart(), rect, m_zoomFactor, tile->id());
    }
#ifdef DEBUG_VERBOSE
    else {
        qDebug() << "tile" << index << "already exists";
    }
#endif
}

void LOView::scheduleVisibleRectUpdate()
{
    if (m_updateTimer.isActive())
        return;

    m_updateTimer.setSingleShot(true);
    m_updateTimer.start(20);
}



void LOView::clearView()
{
    for (auto i = m_tiles.begin(); i != m_tiles.end(); ++i)
        RenderEngine::instance()->dequeueTask(i.value()->id());

    auto i = m_tiles.begin();
    while (i != m_tiles.end()) {
        SGTileItem* sgtile = i.value();
        sgtile->deleteLater();
        i = m_tiles.erase(i);
    }
}

void LOView::updateTileData(AbstractRenderTask* task, QImage img)
{
    int id = task->id();
    for (auto i = m_tiles.begin(); i != m_tiles.end(); ++i) {
        SGTileItem* sgtile = i.value();
        if (sgtile->id() == id) {
            sgtile->setData(img);
            break;
        }
    }
}

void LOView::updateThumbnailModel(AbstractRenderTask* task, QImage img)
{
    int id = task->id();
    if (!m_imageProvider->m_images.contains(id))
        m_imageProvider->m_images.insert(id, img);
    m_partsModel->notifyAboutChanges(id);
}

void LOView::setError(const LibreOfficeError::Error &error)
{
    if (m_error == error)
        return;

    m_error = error;
    Q_EMIT errorChanged();
}

LOView::~LOView()
{
    delete m_partsModel;

    disconnect(RenderEngine::instance(), &RenderEngine::taskRenderFinished,
            this, &LOView::slotTaskRenderFinished);

    // Remove all tasks from rendering queue.
    for (auto i = m_tiles.begin(); i != m_tiles.end(); ++i)
        RenderEngine::instance()->dequeueTask(i.value()->id());
}
