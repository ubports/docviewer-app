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
    }

    return node;
}

//QSGNode *SGTileItem::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *)
//{
//    QSGSimpleTextureNode* node = static_cast<QSGSimpleTextureNode*>(oldNode);
//    QQuickWindow* wnd = window();

//    if (!node && wnd) {
//        if (this->m_state.loadAcquire() == SgstInitial) {
//            m_state.storeRelease(SgstRendering);

//            QtConcurrent::run( [=] {
//                if (!m_document)
//                    return;

//                QImage img;

//                // By this time already can be disposed, so it's better to ckeck again.
//                if (this->m_state.loadAcquire() == SgstRendering)
//                    img = m_document->paintTile(this->area().size(), this->area());

//#ifdef DEBUG_VERBOSE
//                else if (this->m_state.loadAcquire() == SgstDisposed)
//                    qDebug() << "Already disposed:" << m_state.loadAcquire();
//#endif

//                QMetaObject::invokeMethod(this, "renderCallback", Q_ARG(QImage, img));
//            });
//        } else if (m_state.loadAcquire() == SgstActive) {
//            QImage image = m_data;
//            auto texture = wnd->createTextureFromImage(image);
//            node = new QSGSimpleTextureNode();
//            node->setTexture(texture);
//            node->setOwnsTexture(true);
//            node->setRect(m_area);

//#ifdef DEBUG_SHOW_TILE_BORDER
//            auto tileBorderGeometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 8);
//            tileBorderGeometry->setDrawingMode(GL_LINES);
//            tileBorderGeometry->setLineWidth(4);

//            QSGGeometry::Point2D* vertex = tileBorderGeometry->vertexDataAsPoint2D();
//            vertex[0].set(node->rect().left(), node->rect().top());
//            vertex[1].set(node->rect().left(), node->rect().bottom());

//            vertex[2].set(node->rect().right(), node->rect().top());
//            vertex[3].set(node->rect().right(), node->rect().bottom());

//            vertex[4].set(vertex[0].x, vertex[0].y);
//            vertex[5].set(vertex[2].x, vertex[2].y);

//            vertex[6].set(vertex[1].x, vertex[1].y);
//            vertex[7].set(vertex[3].x, vertex[3].y);

//            auto tileBorderMaterial = new QSGFlatColorMaterial;
//            tileBorderMaterial->setColor(Qt::red);

//            auto tileBorderNode = new QSGGeometryNode;

//            tileBorderNode->setGeometry(tileBorderGeometry);
//            tileBorderNode->setFlag(QSGNode::OwnsGeometry);

//            tileBorderNode->setMaterial(tileBorderMaterial);
//            tileBorderNode->setFlag(QSGNode::OwnsMaterial);

//            node->appendChildNode(tileBorderNode);
//#endif
//        }
//    }

//    return node;
//}

void SGTileItem::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickItem::geometryChanged(newGeometry, oldGeometry);
}

