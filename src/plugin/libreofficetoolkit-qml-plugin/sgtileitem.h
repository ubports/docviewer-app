#ifndef SGTILEITEM_H
#define SGTILEITEM_H

#include <QQuickItem>
#include <QQuickWindow>
#include <QSGSimpleTextureNode>

class LODocument;

class SGTileItem : public QQuickItem
{
    Q_OBJECT
public:
    SGTileItem(const QRect &area, LODocument *document, QQuickItem *parent = 0);
    ~SGTileItem();

    inline QRect area() const { return m_area; }
    inline void setArea(const QRect &area) { m_area = area; }

    inline LODocument* document() const { return m_document; }
    inline void setDocument(LODocument* document) { m_document = document; }

protected:
    virtual QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);
    virtual void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);

signals:

private:
    QRect m_area;
    LODocument* m_document;

public slots:
};

#endif // SGTILEITEM_H
