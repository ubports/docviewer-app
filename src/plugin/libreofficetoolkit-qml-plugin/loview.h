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

#ifndef LOVIEW_H
#define LOVIEW_H

#include <QQuickPaintedItem>
#include <QTimer>

class LODocument;
class TileItem;

class LOView : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QQuickItem* parentFlickable READ parentFlickable WRITE setParentFlickable NOTIFY parentFlickableChanged)
    Q_PROPERTY(LODocument* document        READ document        WRITE setDocument        NOTIFY documentChanged)

    // TODO: Implement zoom!
    Q_PROPERTY(qreal       zoomFactor      READ zoomFactor      WRITE setZoomFactor      NOTIFY zoomFactorChanged)

    // TODO: Cache buffer
    //Q_PROPERTY(int       cacheBuffer     READ cacheBuffer     WRITE setCacheBuffer     NOTIFY cacheBufferChanged)

public:
    LOView(QQuickItem *parent = 0);
    ~LOView();

    void        paint(QPainter *painter);

    QQuickItem* parentFlickable() const;
    void        setParentFlickable(QQuickItem* flickable);

    LODocument* document() const;
    void        setDocument(LODocument* doc);

    qreal       zoomFactor() const;
    void        setZoomFactor(qreal &zoom);

Q_SIGNALS:
    void parentFlickableChanged();
    void documentChanged();
    void zoomFactorChanged();

private Q_SLOTS:
    void updateViewSize();
    void updateVisibleRect();
    void scheduleVisibleRectUpdate();

private:
    QQuickItem*             m_parentFlickable;
    LODocument*             m_document;

    qreal                   m_zoomFactor;
    QRect                   m_visibleArea;

    QTimer                  m_updateTimer;

    // TODO: Should we move tiles management in another class (e.g. TileBuffer)?
    QMap<int, TileItem*>    m_tiles;
};

#endif // LOVIEW_H
