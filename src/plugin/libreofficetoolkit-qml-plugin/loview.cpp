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
#include "twips.h"

#include <QPainter>
#include <QImage>
#include <QtCore/qmath.h>

// TODO: Use a QQuickItem and implement painting through
// updatePaintNode(QSGNode * oldNode, UpdatePaintNodeData * data)

// TODO: Should updateViewSize() and updateVisibleRect() merge together?

LOView::LOView(QQuickItem *parent)
    : QQuickPaintedItem(parent)
    , m_parentFlickable(nullptr)
    , m_document(nullptr)
    , m_zoomFactor(1.0)
    , m_visibleArea(0, 0, 0, 0)
{
    Q_UNUSED(parent)

    // Connections for updating the canvas size.
    connect(this, SIGNAL(documentChanged()), this, SLOT(updateViewSize()));
    connect(this, SIGNAL(zoomFactorChanged()), this, SLOT(updateViewSize()));

    connect(this, SIGNAL(parentFlickableChanged()), this, SLOT(updateVisibleRect()));
}

void LOView::paint(QPainter *painter)
{
    qDebug() << "Painting new tiles...";

    // Clean area outside the visible one
    painter->eraseRect(QRect(0, 0, m_visibleArea.right(), m_visibleArea.top()));  // TOP
    painter->eraseRect(QRect(m_visibleArea.left(), m_visibleArea.bottom(), m_visibleArea.right(), this->height() - m_visibleArea.bottom()));    // BOTTOM
    painter->eraseRect(QRect(0, m_visibleArea.top(), m_visibleArea.left(), m_visibleArea.height()));  // LEFT
    painter->eraseRect(QRect(m_visibleArea.right(), m_visibleArea.top(), this->width() - m_visibleArea.right(), m_visibleArea.height()));  // RIGHT

    Q_FOREACH(TileItem* tile, m_tiles) {
       // if (!tile->painted) {
            painter->drawImage(tile->area, tile->texture);
            tile->painted = true;
        //}
    }
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

    connect(m_parentFlickable, SIGNAL(contentXChanged()), this, SLOT(updateVisibleRect()));
    connect(m_parentFlickable, SIGNAL(contentYChanged()), this, SLOT(updateVisibleRect()));

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
void LOView::setZoomFactor(qreal &zoom)
{
    if (m_zoomFactor == zoom)
        return;

    m_zoomFactor = zoom;
    Q_EMIT zoomFactorChanged();
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

    // FIXME: updateVisibleRect (and then the update() requested) are executed
    // when size of the view is still (0,0). For that reason no tile is visible
    // is the content is not flicked.
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
    // TODO: Use cacheBuffer property
    QRect loadingArea(m_visibleArea.x() - (m_visibleArea.width() * 2),
                      m_visibleArea.y() - (m_visibleArea.height() * 2),
                      m_visibleArea.width() * 5,
                      m_visibleArea.height() * 5);

    // TODO: We are using a QMap, which sorts the elements by their index.
    // That means that we can just delete all the first item until we get the
    // first item that intersects the loadingArea.

    // TODO: Do the same described above backwards from the last element.
    if (!m_tiles.isEmpty()) {
        // Delete tiles that are outside the loading area
        QMap<int, TileItem*>::iterator b = m_tiles.begin();
        while (b != m_tiles.end()) {
            if (!loadingArea.intersects(b.value()->area)) {
                qDebug() << "Removing tile indexed as" << b.key();
                b.value()->releaseTexture();
                b = m_tiles.erase(b);
            } else {
                ++b;
            }
        }
    }

    // Calculate tiles grid size
    int numberOfTilesWidth = qCeil(this->width() / TILE_SIZE);

    int startFromWidth = int(m_visibleArea.x() / TILE_SIZE);
    int startFromHeight = int(m_visibleArea.y() / TILE_SIZE);

    int stopToWidth = qCeil(qreal(m_visibleArea.right()) / TILE_SIZE);
    int stopToHeight = qCeil(qreal(m_visibleArea.bottom()) / TILE_SIZE);

    // Generate new visible tiles
    qDebug() << "Generate new visible tiles...";
    for (int r = startFromWidth; r < stopToWidth; r++) {
        for (int c = startFromHeight; c < stopToHeight; c++) {
            QRect tileRect(r * TILE_SIZE, c * TILE_SIZE, TILE_SIZE, TILE_SIZE);
            int index = r * numberOfTilesWidth + c;

            if (!m_tiles.contains(index)) {
                qDebug() << "Creating tile" << r << "x" << c;
                TileItem* tile = new TileItem(tileRect, m_document);

                // Append the tile in the map
                m_tiles.insert(index, tile);

                // Connect the tile to the QQuickPaintedItem's update() slot, so the tile is immediately painted.
                qDebug() << "Connecting tile" << r << "x" << c;
                connect(tile, SIGNAL(textureUpdated()), this, SLOT(update()));
            } else {
                // Just some debugging
                qDebug() << "tile" << r << "x" << c << "already exists";
            }
        }
    }

    // TODO: Generate tiles in the cacheBuffer area
    // (currently called loadingArea).

    // Request a new paint.
    this->update();
}

LOView::~LOView()
{
    //
}
