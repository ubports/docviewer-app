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
#include "tileitem.h"
#include "sgtileitem.h"
#include "twips.h"
#include "config.h"

#include <QPainter>
#include <QImage>
#include <QTimer>
#include <QtCore/qmath.h>

// TODO: Use a QQuickItem and implement painting through
// updatePaintNode(QSGNode * oldNode, UpdatePaintNodeData * data)

LOView::LOView(QQuickItem *parent)
    : QQuickItem(parent) //QQuickPaintedItem(parent)
    , m_parentFlickable(nullptr)
    , m_document(nullptr)
    , m_zoomFactor(1.0)
    , m_cacheBuffer(TILE_SIZE * 3)
    , m_visibleArea(0, 0, 0, 0)
    , m_bufferArea(0, 0, 0, 0)
{
    Q_UNUSED(parent)   

    connect(this, SIGNAL(documentChanged()), this, SLOT(updateViewSize()));
    connect(this, SIGNAL(zoomFactorChanged()), this, SLOT(updateViewSize()));
    connect(this, SIGNAL(parentFlickableChanged()), this, SLOT(updateVisibleRect()));
    connect(this, SIGNAL(cacheBufferChanged()), this, SLOT(updateVisibleRect()));
    connect(&m_updateTimer, SIGNAL(timeout()), this, SLOT(updateVisibleRect()));
}

//void LOView::paint(QPainter *painter)
//{
//    Q_FOREACH(TileItem* tile, m_tiles) {
//        painter->drawImage(tile->area(), tile->texture());
//        tile->setPainted(true);

//#ifdef DEBUG_SHOW_TILE_BORDER
//        painter->drawRect(tile->area());
//#endif
//    }
//}

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

// Return the LODocument rendered by this class
LODocument* LOView::document() const
{
    return m_document;
}

// Set the LODocument
void LOView::setDocument(LODocument *doc)
{
    if (m_document == doc)
        return;

    m_document = doc;
    Q_EMIT documentChanged();
}

// Not used yet.
qreal LOView::zoomFactor() const
{
    return m_zoomFactor;
}

// Not used yet.
void LOView::setZoomFactor(qreal zoom)
{
    if (m_zoomFactor == zoom)
        return;

    m_zoomFactor = zoom;
    Q_EMIT zoomFactorChanged();
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

// Update the size of LOView, according to the size of the loaded document.
void LOView::updateViewSize()
{
    if (!m_document)
        return;

    QSize docSize = m_document->documentSize();

    // FIXME: Area may become too large, resulting in a black texture.
    this->setWidth(Twips::convertTwipsToPixels(docSize.width()) * m_zoomFactor);
    this->setHeight(Twips::convertTwipsToPixels(docSize.height()) * m_zoomFactor);

    // TODO: Consider to use connections to widthChanged and heightChanged
    this->updateVisibleRect();
}

// Update the informations of the currently visible area of the parent
// Flickable, then generate/delete all the required tiles, according to these
// informations.
void LOView::updateVisibleRect()
{
    if (!m_parentFlickable)
        return;

    // Update information about the visible area
    m_visibleArea.setRect(m_parentFlickable->property("contentX").toInt(),
                          m_parentFlickable->property("contentY").toInt(),
                          m_parentFlickable->width(),
                          m_parentFlickable->height());

    // Update information about the buffer area
    m_bufferArea.setRect(qMax(0, m_visibleArea.x() - this->cacheBuffer()),
                         qMax(0, m_visibleArea.y() - this->cacheBuffer()),
                         qMin(int(this->width() - m_bufferArea.x()), m_visibleArea.width() + (this->cacheBuffer() * 2)),
                         qMin(int(this->height() - m_bufferArea.y()), m_visibleArea.height() + (this->cacheBuffer() * 2)));

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

            sgtile->deleteLater();
            i = m_tiles.erase(i);
        }
    }

    // Number of tiles per row
    int tilesPerWidth           = qCeil(this->width() / TILE_SIZE);

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

    this->generateTiles(visiblesFromWidth, visiblesFromHeight, visiblesToWidth, visiblesToHeight, tilesPerWidth);
    this->generateTiles(bufferFromWidth, bufferFromHeight, bufferToWidth, bufferToHeight, tilesPerWidth);
}

void LOView::generateTiles(int x1, int y1, int x2, int y2, int tilesPerWidth)
{
    for (int x = x1; x < x2; x++) {
        for (int y = y1; y < y2; y++) {
            QRect tileRect(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
            int index = y * tilesPerWidth + x;

            this->createTile(index, tileRect);
        }
    }
}

void LOView::createTile(int index, QRect rect)
{
    if (!m_tiles.contains(index)) {
#ifdef DEBUG_VERBOSE
        qDebug() << "Creating tile indexed as" << index;
#endif

        auto tile = new SGTileItem(rect, m_document, this);
        // TODO When we should update with SG?
        // connect(tile, SIGNAL(textureChanged()), this, SLOT(update()));

        // Append the tile in the map
        m_tiles.insert(index, tile);
    }
#ifdef DEBUG_VERBOSE
    else {
        qDebug() << "tile" << x << "x" << y << "already exists";
    }
#endif
}

void LOView::scheduleVisibleRectUpdate()
{
    if (!m_updateTimer.isActive())
        m_updateTimer.start(20);
}

LOView::~LOView()
{
    //
}
