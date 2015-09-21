#include "sgtileitem.h"

#include "lodocument.h"
#include "config.h"

#ifdef DEBUG_SHOW_TILE_BORDER
#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>
#endif

int SGTileItem::s_idCounter = 0xDEAD0000;

SGTileItem::SGTileItem(const QRect& area, QQuickItem *parent)
    : QQuickItem(parent)
    , m_area(area)
    , m_id (s_idCounter++)
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
        QImage image = m_data;
        auto texture = wnd->createTextureFromImage(image);
        node = new QSGSimpleTextureNode();
        node->setTexture(texture);
        node->setOwnsTexture(true);
        node->setRect(m_area);

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
void SGTileItem::drawTileBorders(QSGNode* basicNode)
{
    auto node = basicNode;
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
