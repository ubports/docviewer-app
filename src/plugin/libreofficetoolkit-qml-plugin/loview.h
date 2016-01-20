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
#include "lorendertask.h"
#include "../../app/renderengine.h"
#include "lopartsmodel.h"
#include "lopartsimageprovider.h"

class LODocument;
class SGTileItem;
class LOZoom;

class LOView : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QQuickItem*              parentFlickable    READ parentFlickable WRITE setParentFlickable NOTIFY parentFlickableChanged)
    Q_PROPERTY(LODocument*              document           READ document        /*WRITE setDocument*/    NOTIFY documentChanged)
    Q_PROPERTY(int                      currentPart        READ currentPart     WRITE setCurrentPart     NOTIFY currentPartChanged)
    Q_PROPERTY(LOPartsModel*            partsModel         READ partsModel                               NOTIFY partsModelChanged)
    Q_PROPERTY(LOZoom*                  zoomSettings       READ zoomSettings)
    Q_PROPERTY(int                      cacheBuffer        READ cacheBuffer     WRITE setCacheBuffer     NOTIFY cacheBufferChanged)
    Q_PROPERTY(LibreOfficeError::Error  error              READ error                                    NOTIFY errorChanged)

public:
    LOView(QQuickItem *parent = 0);
    ~LOView();

    QQuickItem* parentFlickable() const;
    void setParentFlickable(QQuickItem* flickable);

    LODocument* document() const;

    LOPartsModel* partsModel() const;

    LOZoom* zoomSettings() const;

    int currentPart();
    void setCurrentPart(int index);

    int cacheBuffer() const;
    void setCacheBuffer(int cacheBuffer);

    LibreOfficeError::Error error() const;

    Q_INVOKABLE void initializeDocument(const QString& path);

    Q_INVOKABLE bool adjustZoomToWidth();
    Q_INVOKABLE bool adjustZoomToHeight();
    Q_INVOKABLE bool adjustAutomaticZoom();

Q_SIGNALS:
    void parentFlickableChanged();
    void documentChanged();
    void partsModelChanged();
    void currentPartChanged();
    void cacheBufferChanged();
    void errorChanged();

private Q_SLOTS:
    void updateViewSize();
    void updateVisibleRect();
    void scheduleVisibleRectUpdate();
    void invalidateAllTiles();
    void slotTaskRenderFinished(AbstractRenderTask* task, QImage img);

private:

    QQuickItem*                 m_parentFlickable;
    QSharedPointer<LODocument>  m_document;
    LOZoom*                     m_zoomSettings;
    LOPartsModel*               m_partsModel; // TODO MB move to document.
    LOPartsImageProvider*       m_imageProvider; // The QQmlEngine takes ownership of provider.

    int                         m_currentPart;
    int                         m_cacheBuffer;

    QRect                       m_visibleArea;
    QRect                       m_bufferArea;

    LibreOfficeError::Error     m_error;

    QTimer                      m_updateTimer;

    QMap<int, SGTileItem*>      m_tiles;

private:
    void generateTiles(int x1, int y1, int x2, int y2, int tilesPerWidth, int tilesPerHeight);
    void createTile(int index, const QRect& rect);
    void clearView();
    TileRenderTask* createTask(const QRect& rect, int id) const;
    void updateTileData(AbstractRenderTask* task, QImage img);
    void updateThumbnailModel(AbstractRenderTask* task, QImage img);

    void setError(const LibreOfficeError::Error &error);
};

#endif // LOVIEW_H
