#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include <QObject>
#include <QImage>
#include <QSharedPointer>
#include <QHash>
#include <QQueue>

#include "lodocument.h"

struct EngineTask
{
    int id;
    QRect area;
    QSharedPointer<LODocument> document;

public:
    EngineTask(const QSharedPointer<LODocument>& d, const QRect& a, int i):
    id(i),
    area(a),
    document(d)
    { }
};

class RenderEngine : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(RenderEngine)

    static RenderEngine* s_instance;
    RenderEngine();

    const int DefaultIdealThreadCount = 2;

public:
    void enqueueTask(const QSharedPointer<LODocument>& doc, const QRect& area, int id);
    void dequeueTask(int id);

    static RenderEngine* instance() {
        if(!s_instance)
            s_instance = new RenderEngine();
        return s_instance;
    }

Q_SIGNALS:
    void renderFinished(int id, QImage img);

private:
    Q_INVOKABLE void internalRenderCallback(int id, QImage img);
    void doNextTask();

private:
    QHash<int, QSharedPointer<LODocument> > m_requests;
    QQueue<EngineTask> m_queue;
    int m_activeTaskCount;
    int m_idealThreadCount;
};

#endif // RENDERENGINE_H
