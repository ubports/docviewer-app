#ifndef SGTILEITEM_H
#define SGTILEITEM_H

#include <QQuickItem>
#include <QImage>

#include "config.h"

class LODocument;

#ifdef DEBUG_SHOW_TILE_BORDER
class QSGSimpleTextureNode;
#endif

class SGTileItem : public QQuickItem
{
    Q_OBJECT
public:
    SGTileItem(const QRect& area, int id, QQuickItem *parent = 0);
    ~SGTileItem();

    inline const QRect& area() { return m_area; }
    inline void setArea(const QRect& rect) { m_area = rect; }

    inline int id() { return m_id; }
    inline void setId(int id) { m_id = id; }

    inline QImage data() { return m_data; }
    inline void setData(QImage data) { m_data = data; update(); }

protected:
    virtual QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);
    virtual void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);

private:
#ifdef DEBUG_SHOW_TILE_BORDER
    void drawTileBorders(QSGSimpleTextureNode *parentNode);
#endif

private:
    QRect m_area;
    QImage m_data;
    int m_id;
};

#endif // SGTILEITEM_H
