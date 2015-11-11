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

#include <QQuickItem>
#include <QTimer>
#include <QSharedPointer>
#include <QQmlContext>
#include <QQmlEngine>

#include "loerror.h"
#include "renderengine.h"
#include "lopartsmodel.h"
#include "lopartsimageprovider.h"

class LODocument;
class SGTileItem;

class LOView : public QQuickItem
{
    Q_OBJECT
    Q_ENUMS(ZoomMode)
    Q_PROPERTY(QQuickItem*              parentFlickable READ parentFlickable WRITE setParentFlickable NOTIFY parentFlickableChanged)
    Q_PROPERTY(LODocument*              document        READ document        /*WRITE setDocument*/    NOTIFY documentChanged)
    Q_PROPERTY(LOPartsModel*            partsModel      READ partsModel                               NOTIFY partsModelChanged)
    Q_PROPERTY(qreal                    zoomFactor      READ zoomFactor      WRITE setZoomFactor      NOTIFY zoomFactorChanged)
    Q_PROPERTY(ZoomMode                 zoomMode        READ zoomMode                                 NOTIFY zoomModeChanged)
    Q_PROPERTY(int                      cacheBuffer     READ cacheBuffer     WRITE setCacheBuffer     NOTIFY cacheBufferChanged)
    Q_PROPERTY(LibreOfficeError::Error  error           READ error                                    NOTIFY errorChanged)

public:
    LOView(QQuickItem *parent = 0);
    ~LOView();

    enum ZoomMode {
        FitToWidth,
        Manual
    };

    QQuickItem* parentFlickable() const;
    void        setParentFlickable(QQuickItem* flickable);

    Q_INVOKABLE void initializeDocument(const QString& path);

    LODocument* document() const;
    LOPartsModel* partsModel() const;

    qreal       zoomFactor() const;
    void        setZoomFactor(const qreal zoom);

    ZoomMode    zoomMode() const;

    int         cacheBuffer() const;
    void        setCacheBuffer(int cacheBuffer);

    LibreOfficeError::Error error() const;

    Q_INVOKABLE void adjustZoomToWidth();

Q_SIGNALS:
    void parentFlickableChanged();
    void documentChanged();
    void partsModelChanged();
    void zoomFactorChanged();
    void zoomModeChanged();
    void cacheBufferChanged();
    void errorChanged();

private Q_SLOTS:
    void updateViewSize();
    void updateVisibleRect();
    void scheduleVisibleRectUpdate();
    void invalidateAllTiles();

    void slotTileRenderFinished(int id, QImage img);
    void slotThumbnailRenderFinished(int id, QImage img);

private:

    QQuickItem*                 m_parentFlickable;
    QSharedPointer<LODocument>  m_document;
    LOPartsModel*               m_partsModel; // TODO MB move to document.
    LOPartsImageProvider*       m_imageProvider; // The QQmlEngine takes ownership of provider.

    qreal                       m_zoomFactor;
    ZoomMode                    m_zoomMode;
    int                         m_cacheBuffer;

    QRect                       m_visibleArea;
    QRect                       m_bufferArea;

    LibreOfficeError::Error     m_error;

    QTimer                      m_updateTimer;

    QMap<int, SGTileItem*>      m_tiles;

    void generateTiles(int x1, int y1, int x2, int y2, int tilesPerWidth);
    void createTile(int index, QRect rect);
    void setZoomMode(const ZoomMode zoomMode);
    void clearView();

    void setError(const LibreOfficeError::Error &error);
};

#endif // LOVIEW_H
