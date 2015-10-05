#include "renderengine.h"
#include <QtConcurrent/QtConcurrent>
#include <QThread>

RenderEngine* RenderEngine::s_instance = nullptr;

RenderEngine::RenderEngine():
    QObject(nullptr),
    m_activeTaskCount(0),
    m_enabled(true),
    m_lastPart(-1)
{
    int itc = QThread::idealThreadCount();
    m_idealThreadCount = itc == -1 ? DefaultIdealThreadCount : itc;

    connect(this, SIGNAL(enabledChanged()), this, SLOT(doNextTask()));
}

void RenderEngine::enqueueTask(const QSharedPointer<LODocument>& doc, const QRect& area, const qreal &zoom, int id)
{
    Q_ASSERT(doc != nullptr);

    m_queue.enqueue(EngineTask(doc, area, zoom, id));

    doNextTask();
}

void RenderEngine::enqueueTask(const QSharedPointer<LODocument> &doc, int part, qreal size, int id)
{
    Q_ASSERT(doc != nullptr);

    m_queue.enqueue(EngineTask(doc, part, size, id));

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

void RenderEngine::internalRenderCallback(int id, QImage img, bool isThumbnail)
{
    m_activeTaskCount--;
    if (isThumbnail)
        Q_EMIT thumbnailRenderFinished(id, img);
    else Q_EMIT renderFinished(id, img);
    doNextTask();
}

void RenderEngine::doNextTask()
{
#ifdef DEBUG_VERBOSE
    qDebug() << " ---- doNextTask" << m_activeTaskCount << m_queue.count();
#endif

    // Check for too much threads or empty queue.
    if (m_activeTaskCount >= m_idealThreadCount || !m_queue.count() || !m_enabled)
        return;

    // We should avoid different part rendering in the same time.
    if (m_activeTaskCount && m_queue.head().part != m_lastPart)
        return;

    m_activeTaskCount++;
    EngineTask task = m_queue.dequeue();

    // Set correct part.
    m_lastPart = task.part;
    task.document->setDocumentPart(m_lastPart);

    QtConcurrent::run( [=] {
        QImage img = task.isThumbnail ?
                    task.document->paintThumbnail(task.size) :
                    task.document->paintTile(task.area.size(), task.area, task.zoom);
        QMetaObject::invokeMethod(this, "internalRenderCallback",
                                  Q_ARG(int, task.id), Q_ARG(QImage, img), Q_ARG(bool, task.isThumbnail));
    });
}
