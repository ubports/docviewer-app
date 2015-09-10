#include "sgtileitem.h"

#include "lodocument.h"
#include "config.h"

SGTileItem::SGTileItem(const QRect &area, LODocument *document, QQuickItem *parent)
    : QQuickItem(parent)
    , m_area(area)
    , m_document(document)
    , m_state(SgstInitial)
{
    setFlag(ItemHasContents, true);
}

SGTileItem::~SGTileItem()
{ }

void SGTileItem::dispose()
{
    if (m_state.loadAcquire() != SgstRendering)
        deleteLater();
    m_state.storeRelease(SgstDisposed);

#ifdef DEBUG_VERBOSE
    qDebug() << "---- dispose called: " << this << m_state;
#endif
}

QSGNode *SGTileItem::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *)
{
    QSGSimpleTextureNode* node = static_cast<QSGSimpleTextureNode*>(oldNode);
    QQuickWindow* wnd = window();

    if (!node && wnd) {
        if (this->m_state.loadAcquire() == SgstInitial) {
            m_state.storeRelease(SgstRendering);

            QtConcurrent::run( [=] {
                if (!m_document)
                    return;

                QImage img;

                // By this time already can be disposed, so it's better to ckeck again.
                if (this->m_state.loadAcquire() == SgstRendering)
                    img = m_document->paintTile(this->area().size(), this->area());

#ifdef DEBUG_VERBOSE
                else if (this->m_state.loadAcquire() == SgstDisposed)
                    qDebug() << "Already disposed:" << m_state.loadAcquire();
#endif

                QMetaObject::invokeMethod(this, "renderCallback", Q_ARG(QImage, img));
            });
        } else if (m_state.loadAcquire() == SgstActive) {
            QImage image = m_data;
            auto texture = wnd->createTextureFromImage(image);
            node = new QSGSimpleTextureNode();
            node->setTexture(texture);
            node->setOwnsTexture(true);
            node->setRect(m_area);
        }
    }

#ifdef DEBUG_SHOW_TILE_BORDER
    // TODO: Tile border rendering
#endif

    return node;
}

void SGTileItem::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickItem::geometryChanged(newGeometry, oldGeometry);
}

void SGTileItem::renderCallback(QImage image)
{
    if (m_state.loadAcquire() == SgstRendering) {
        m_data = image;
        m_state.storeRelease(SgstActive);
        update();
    } else deleteLater();
}

