#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include <QObject>
#include <QImage>
#include <QSharedPointer>
#include <QHash>

#include "lodocument.h"

class RenderEngine : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(RenderEngine)

    static RenderEngine* s_instance;
    RenderEngine(): QObject(nullptr) { }

public:
    void renderArea(const QSharedPointer<LODocument>& doc, const QRect& area, int id);

    static RenderEngine* instance() {
        if(!s_instance)
            s_instance = new RenderEngine();
        return s_instance;
    }

Q_SIGNALS:
    void renderFinished(int id, QImage img);

private:
    Q_INVOKABLE void internalRenderCallback(int id, QImage img);

private:
    QHash<int, QSharedPointer<LODocument> > m_requests;

};

#endif // RENDERENGINE_H
