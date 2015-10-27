#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include <QObject>
#include <QImage>
#include <QSharedPointer>
#include <QHash>
#include <QQueue>
#include <QAtomicInt>

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
    void tileRenderFinished(int id, QImage img);
    void thumbnailRenderFinished(int id, QImage img);
    void enabledChanged();

private:
    Q_INVOKABLE void internalRenderCallback(AbstractRenderTask* task, QImage img);
    void disposeTask(AbstractRenderTask* task);
    void doNextTask();

private:
    QQueue<AbstractRenderTask*> m_queue;
    int m_activeTaskCount;
    int m_idealThreadCount;
    int m_lastPart;
};

#endif // RENDERENGINE_H
