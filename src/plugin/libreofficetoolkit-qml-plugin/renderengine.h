#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include <QObject>
#include <QImage>
#include <QSharedPointer>
#include <QHash>
#include <QQueue>
#include <QAtomicInt>

#include "lodocument.h"

// TODO replace with class.


// TODO Need more OOP here.
struct EngineTask
{
    int id;
    int part;
    QSharedPointer<LODocument> document;
    // Used in thumbnail rendering.
    qreal size;
    // Used in tile rendering.
    QRect area;
    qreal zoom;
    // Internal.
    bool isThumbnail;
public:

    EngineTask(const QSharedPointer<LODocument>& d, int p, const QRect& a, const qreal& z, int i):
        id(i),
        part(p),
        document(d),
        size(0),
        area(a),
        zoom(z),
        isThumbnail(false)
    { }

    EngineTask(const QSharedPointer<LODocument>& d, int p, qreal s, int i):
        id(i),
        part(p),
        document(d),
        size(s),
        area(),
        zoom(0),
        isThumbnail(true)
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
    void enqueueTask(const QSharedPointer<LODocument>& doc, int part, const QRect& area, const qreal& zoom, int id);
    void enqueueTask(const QSharedPointer<LODocument>& doc, int part, qreal size, int id);
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

    // TODO DELETE
    //int activeTaskCount() { return m_activeTaskCount; }

//    bool enabled() { return m_enabled.loadAcquire(); }
//    void setEnabled(bool enabled) {
//        if (m_enabled.loadAcquire() == enabled)
//            return;

//        m_enabled.storeRelease(enabled);
//        Q_EMIT enabledChanged();
//    }

Q_SIGNALS:
    void renderFinished(int id, QImage img);
    void thumbnailRenderFinished(int id, QImage img);
    void enabledChanged();

private:
    Q_INVOKABLE void internalRenderCallback(int id, QImage img, bool isThumbnail);

private slots:
    void doNextTask();

private:
    QQueue<EngineTask> m_queue;
    int m_activeTaskCount;
    int m_idealThreadCount;
    int m_lastPart;

    QAtomicInt m_enabled;
};

#endif // RENDERENGINE_H
