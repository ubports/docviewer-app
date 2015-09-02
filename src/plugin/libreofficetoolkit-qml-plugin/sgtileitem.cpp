#include "sgtileitem.h"

#include "lodocument.h"
#include "config.h"

SGTileItem::SGTileItem(const QRect &area, LODocument *document, QQuickItem *parent)
    : QQuickItem(parent)
    , m_area(area)
    , m_document(document)
{
    setFlag(ItemHasContents, true);
}

SGTileItem::~SGTileItem()
{ }

QSGNode *SGTileItem::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *)
{
    QSGSimpleTextureNode* node = static_cast<QSGSimpleTextureNode*>(oldNode);
    QQuickWindow* wnd = window();

    if (!node && wnd)
    {
        QImage image;
        // image.load("/home/qtros/screenshot.png");
        image = m_document->paintTile(this->area().size(), this->area());
        auto texture = wnd->createTextureFromImage(image);

        node = new QSGSimpleTextureNode();
        node->setTexture(texture);
        node->setOwnsTexture(true);
        node->setRect(m_area);
    }

    return node;
}

void SGTileItem::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickItem::geometryChanged(newGeometry, oldGeometry);
}

