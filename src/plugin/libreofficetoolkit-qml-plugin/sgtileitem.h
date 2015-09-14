#ifndef SGTILEITEM_H
#define SGTILEITEM_H

#include <QQuickItem>
#include <QQuickWindow>
#include <QSGSimpleTextureNode>
#include <QImage>
#include <QtConcurrent/QtConcurrent>
#include <QAtomicInteger>

enum SGTileItemState
{
    SgstInitial = 1,
    SgstRendering,
    SgstActive,
    SgstDisposed
};

class LODocument;

class SGTileItem : public QQuickItem
{
    Q_OBJECT
public:
    SGTileItem(const QRect &area, LODocument *document, const qreal &zoom = 1.0, QQuickItem *parent = 0);
    ~SGTileItem();

    inline QRect area() const { return m_area; }
    inline void setArea(const QRect &area) { m_area = area; }

    inline LODocument* document() const { return m_document; }
    inline void setDocument(LODocument* document) { m_document = document; }

    inline qreal zoomFactor() const { return m_zoomFactor; }
    void setZoomFactor(const qreal &zoom) { m_zoomFactor = zoom; }

    void dispose();

protected:
    virtual QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);
    virtual void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);

private:
    Q_INVOKABLE void renderCallback(QImage image);

signals:

private:
    QRect m_area;
    LODocument* m_document;
    qreal m_zoomFactor;
    QImage m_data;
    QAtomicInteger<int> m_state;

public slots:
};

#endif // SGTILEITEM_H
