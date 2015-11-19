#include "sgtileitem.h"
#include "lodocument.h"

#include <QQuickWindow>
#include <QSGSimpleTextureNode>

#ifdef DEBUG_SHOW_TILE_BORDER
#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>
#endif

SGTileItem::SGTileItem(const QRect& area, qreal zoom, int id, QQuickItem *parent)
    : QQuickItem(parent)
    , m_area(area)
    , m_zoomFactor(zoom)
    , m_id (id)
{
    setFlag(ItemHasContents, true);
}

SGTileItem::~SGTileItem()
{ }

QSGNode *SGTileItem::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *)
{
    QSGSimpleTextureNode* node = static_cast<QSGSimpleTextureNode*>(oldNode);
    QQuickWindow* wnd = window();

    if (!node && wnd && !m_data.isNull()) {
        auto texture = wnd->createTextureFromImage(m_data);
        node = new QSGSimpleTextureNode();
        node->setTexture(texture);
        node->setOwnsTexture(true);
        node->setRect(m_area);

        // We don't need anymore QImage's data
        m_data = QImage();

#ifdef DEBUG_SHOW_TILE_BORDER
        drawTileBorders(node);
#endif
    }

    return node;
}

void SGTileItem::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickItem::geometryChanged(newGeometry, oldGeometry);
}

#ifdef DEBUG_SHOW_TILE_BORDER
void SGTileItem::drawTileBorders(QSGSimpleTextureNode* parentNode)
{
    auto node = parentNode;
    auto tileBorderGeometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 8);
    tileBorderGeometry->setDrawingMode(GL_LINES);
    tileBorderGeometry->setLineWidth(4);

    QSGGeometry::Point2D* vertex = tileBorderGeometry->vertexDataAsPoint2D();
    vertex[0].set(node->rect().left(), node->rect().top());
    vertex[1].set(node->rect().left(), node->rect().bottom());

    vertex[2].set(node->rect().right(), node->rect().top());
    vertex[3].set(node->rect().right(), node->rect().bottom());

    vertex[4].set(vertex[0].x, vertex[0].y);
    vertex[5].set(vertex[2].x, vertex[2].y);

    vertex[6].set(vertex[1].x, vertex[1].y);
    vertex[7].set(vertex[3].x, vertex[3].y);

    auto tileBorderMaterial = new QSGFlatColorMaterial;
    tileBorderMaterial->setColor(Qt::red);

    auto tileBorderNode = new QSGGeometryNode;

    tileBorderNode->setGeometry(tileBorderGeometry);
    tileBorderNode->setFlag(QSGNode::OwnsGeometry);

    tileBorderNode->setMaterial(tileBorderMaterial);
    tileBorderNode->setFlag(QSGNode::OwnsMaterial);

    node->appendChildNode(tileBorderNode);
}
#endif
