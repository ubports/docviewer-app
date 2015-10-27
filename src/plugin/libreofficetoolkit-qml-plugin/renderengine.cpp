#include "renderengine.h"
#include <QtConcurrent/QtConcurrent>
#include <QThread>

RenderEngine* RenderEngine::s_instance = nullptr;

RenderEngine::RenderEngine():
    QObject(nullptr),
    m_activeTaskCount(0),
    m_lastPart(-1)
{
    int itc = QThread::idealThreadCount();
    m_idealThreadCount = itc == -1 ? DefaultIdealThreadCount : itc;

    // For QMetaObject::invoke.
    qRegisterMetaType<AbstractRenderTask*>();
}

void RenderEngine::enqueueTileTask(const QSharedPointer<LODocument>& doc, int part, const QRect& area, qreal zoom, int id)
{
    Q_ASSERT(doc != nullptr);

    TileRenderTask* task = new TileRenderTask();
    task->setId(id);
    task->setPart(part);
    task->setDocument(doc);
    task->setArea(area);
    task->setZoom(zoom);

    enqueueTask(task);
}

void RenderEngine::enqueueThumbnailTask(const QSharedPointer<LODocument> &doc, int part, qreal size, int id)
{
    Q_ASSERT(doc != nullptr);

    ThumbnailRenderTask* task = new ThumbnailRenderTask();
    task->setId(id);
    task->setPart(part);
    task->setDocument(doc);
    task->setSize(size);

    enqueueTask(task);
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
            disposeTask(task);
            break;
        }
    }
}

void RenderEngine::internalRenderCallback(AbstractRenderTask* task, QImage img)
{
    m_activeTaskCount--;

    switch (task->type())
    {
    case RttTile:
        Q_EMIT tileRenderFinished(task->id(), img);
        break;
    case RttImpressThumbnail:
        Q_EMIT thumbnailRenderFinished(task->id(), img);
        break;
    }

    disposeTask(task);

    doNextTask();
}

void RenderEngine::disposeTask(AbstractRenderTask *task)
{
    delete task;
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

    // LoRenderTask requires special check.
    if (task->type() == RttTile || task->type() == RttImpressThumbnail) {
        LoRenderTask* loTask = static_cast<LoRenderTask*>(task);

        if (m_activeTaskCount && loTask->part() != m_lastPart)
            return;

        // Set correct part.
        m_lastPart = loTask->part();
        loTask->document()->setDocumentPart(m_lastPart);
    }

    m_activeTaskCount++;
    m_queue.dequeue();

    QtConcurrent::run( [=] {
        QImage img = task->doWork();
        QMetaObject::invokeMethod(this, "internalRenderCallback",
                                  Q_ARG(AbstractRenderTask*, task), Q_ARG(QImage, img));
    });
}
