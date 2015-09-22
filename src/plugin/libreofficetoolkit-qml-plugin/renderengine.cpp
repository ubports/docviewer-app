#include "renderengine.h"
#include <QtConcurrent/QtConcurrent>
#include <QThread>

RenderEngine* RenderEngine::s_instance = nullptr;

RenderEngine::RenderEngine():
    QObject(nullptr),
    m_activeTaskCount(0)
{
    int itc = QThread::idealThreadCount();
    m_idealThreadCount = itc == -1 ? DefaultIdealThreadCount : itc;
}

void RenderEngine::enqueueTask(const QSharedPointer<LODocument>& doc, const QRect& area, const qreal &zoom, int id)
{
    Q_ASSERT(doc != nullptr);

    m_queue.enqueue(EngineTask(doc, area, zoom, id));

    doNextTask();
}

void RenderEngine::dequeueTask(int id)
{
    for (int i = 0; i < m_queue.size(); i++)
        if (m_queue.at(i).id == id) {
            m_queue.removeAt(i);
            break;
        }
}

void RenderEngine::internalRenderCallback(int id, QImage img)
{
    m_activeTaskCount--;
    Q_EMIT renderFinished(id, img);
    doNextTask();
}

void RenderEngine::doNextTask()
{
#ifdef DEBUG_VERBOSE
    qDebug() << " ---- doNextTask" << m_activeTaskCount << m_queue.count();
#endif

    if (m_activeTaskCount >= m_idealThreadCount || !m_queue.count())
        return;

    m_activeTaskCount++;
    auto task = m_queue.dequeue();

    QtConcurrent::run( [=] {
        QImage img = task.document->paintTile(task.area.size(), task.area, task.zoom);
        QMetaObject::invokeMethod(this, "internalRenderCallback", Q_ARG(int, task.id), Q_ARG(QImage, img));
    });
}
