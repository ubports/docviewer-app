#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include <QObject>
#include <QImage>
#include <QSharedPointer>
#include <QHash>
#include <QQueue>
#include <QAtomicInt>
#include <QList>

#include "lodocument.h"
#include "rendertask.h"

class RenderEngine : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(RenderEngine)

    static RenderEngine* s_instance;
    RenderEngine();

    const int DefaultIdealThreadCount = 2;

public:
    void enqueueTileTask(const QSharedPointer<LODocument>& doc, int part, const QRect& area, qreal zoom, int id);
    void enqueueThumbnailTask(const QSharedPointer<LODocument>& doc, int part, qreal size, int id);
    void enqueueTask(AbstractRenderTask* task);
    void dequeueTask(int id);

    static RenderEngine* instance() {
        if(!s_instance)
            s_instance = new RenderEngine();
        return s_instance;
    }

    static int getNextId() {
        static int idCounter = 0xDEAD0000;
        return idCounter++;
    }

Q_SIGNALS:
    void taskRenderFinished(AbstractRenderTask* task, QImage img);

private:
    Q_INVOKABLE void internalRenderCallback(AbstractRenderTask* task, QImage img);
    void doNextTask();
    void disposeLater(AbstractRenderTask* task); // Delayed deletion, must be used in pair with "doDispose".
    void doDispose();                            // Deletes marked objects (disposeLater).

private:
    QQueue<AbstractRenderTask*> m_queue;
    int m_activeTaskCount;
    int m_idealThreadCount;

    AbstractRenderTask* m_lastTask; // WARNING: valid only when: m_activeTaskCount > 0.
    QList<AbstractRenderTask*> m_disposedTasks;
};

#endif // RENDERENGINE_H
