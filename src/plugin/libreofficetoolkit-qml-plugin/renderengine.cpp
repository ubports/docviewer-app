#include "renderengine.h"
#include <QtConcurrent/QtConcurrent>

RenderEngine* RenderEngine::s_instance = nullptr;

void RenderEngine::renderTile(QSharedPointer<LODocument> doc, const QRect& area, int id)
{
    Q_ASSERT(doc != nullptr);

    m_requests.insert(id, doc);

    QtConcurrent::run( [=] {
        QImage img = doc->paintTile(area.size(), area);
        QMetaObject::invokeMethod(this, "renderCallback", Q_ARG(int, id), Q_ARG(QImage, img));
    });
}

void RenderEngine::renderCallback(int id, QImage img)
{
    m_requests.remove(id);
    Q_EMIT renderFinished(id, img);
}
