#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include <QObject>
#include <QImage>
#include <QSharedPointer>
#include <QHash>
#include <QQueue>
#include <QAtomicInt>

#include "lodocument.h"

struct EngineTask
{
    int id;
    QRect area;
    qreal zoom;
    QSharedPointer<LODocument> document;

public:
    EngineTask(const QSharedPointer<LODocument>& d, const QRect& a, const qreal& z, int i):
    id(i),
    area(a),
    zoom(z),
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
    void enqueueTask(const QSharedPointer<LODocument>& doc, const QRect& area, const qreal& zoom, int id);
    void dequeueTask(int id);

    static RenderEngine* instance() {
        if(!s_instance)
            s_instance = new RenderEngine();
        return s_instance;
    }

    int activeTaskCount() { return m_activeTaskCount; }

    bool enabled() { return m_enabled.loadAcquire(); }
    void setEnabled(bool enabled) {
        if (m_enabled.loadAcquire() == enabled)
            return;

        m_enabled.storeRelease(enabled);
        Q_EMIT enabledChanged();
    }

Q_SIGNALS:
    void renderFinished(int id, QImage img);
    void enabledChanged();

private:
    Q_INVOKABLE void internalRenderCallback(int id, QImage img);

private slots:
    void doNextTask();

private:
    QQueue<EngineTask> m_queue;
    int m_activeTaskCount;
    int m_idealThreadCount;

    QAtomicInt m_enabled;
};

#endif // RENDERENGINE_H
