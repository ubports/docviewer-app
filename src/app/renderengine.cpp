#include "renderengine.h"
#include <QtConcurrent/QtConcurrent>
#include <QThread>

RenderEngine* RenderEngine::s_instance = nullptr;

RenderEngine::RenderEngine():
    QObject(nullptr)
    ,m_activeTaskCount(0)
    ,m_lastTask(nullptr)
{
    int itc = QThread::idealThreadCount();
    m_idealThreadCount = itc == -1 ? DefaultIdealThreadCount : itc;

    // For QMetaObject::invoke.
    qRegisterMetaType<AbstractRenderTask*>();
}

void RenderEngine::enqueueTask(AbstractRenderTask *task)
{
    m_queue.enqueue(task);
    doNextTask();
}

void RenderEngine::dequeueTask(int id)
{
    for (int i = 0; i < m_queue.size(); i++) {
        auto task = m_queue.at(i);
        if (task->id() == id) {
            m_queue.removeAt(i);

            if (!task->isOwnedByCaller())
                disposeLater(task);

            break;
        }
    }
}

void RenderEngine::internalRenderCallback(AbstractRenderTask* task, QImage img)
{
    m_activeTaskCount--;

    if (!task->isOwnedByCaller())
        disposeLater(task);

    // Notify about result.
    emit taskRenderFinished(task, img);

    doNextTask();

    if (!m_activeTaskCount) {
        m_lastTask = nullptr;
        doDispose();
    }
}

void RenderEngine::disposeLater(AbstractRenderTask *task)
{
    m_disposedTasks.append(task);
}

void RenderEngine::doDispose()
{
    for (int i = 0; i < m_disposedTasks.size(); ++i)
        delete m_disposedTasks.at(i);
    m_disposedTasks.clear();
}

void RenderEngine::doNextTask()
{
#ifdef DEBUG_VERBOSE
    qDebug() << " ---- doNextTask" << m_activeTaskCount << m_queue.count();
#endif

    // Check for too much threads or empty queue.
    if (m_activeTaskCount >= m_idealThreadCount || !m_queue.count())
        return;

    AbstractRenderTask* task = m_queue.head();

    // If some tasks already in progress, we should ask task about
    // compatibility of parallel execution with last the task.
    if (m_activeTaskCount && !task->canBeRunInParallel(m_lastTask))
        return;

    task->prepare();

    m_activeTaskCount++;
    m_lastTask = m_queue.dequeue();

    QtConcurrent::run( [=] {
        QImage img = task->doWork();
        QMetaObject::invokeMethod(this, "internalRenderCallback",
                                  Q_ARG(AbstractRenderTask*, task), Q_ARG(QImage, img));
    });
}
