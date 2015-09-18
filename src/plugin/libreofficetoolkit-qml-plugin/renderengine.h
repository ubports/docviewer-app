#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include <QObject>
#include <QImage>
#include <QSharedPointer>

#include "lodocument.h"

struct RenderRequestData
{
    int id;

};

class RenderEngine : public QObject
{
    Q_OBJECT

public:
    void renderTile(QSharedPointer<LODocument> doc, const QRect& area, int id);

Q_SIGNALS:
    void renderFinished(int id, QImage img);

private:
    void renderCallback(int id, QImage img);

private:

private:
    static RenderEngine* s_instance;
    RenderEngine(): QObject(nullptr) { }
    RenderEngine( const RenderEngine& ): QObject(nullptr) {}
    RenderEngine& operator=( RenderEngine& );
public:
    static RenderEngine* instance() {
        if(!s_instance)
            s_instance = new RenderEngine();
        return s_instance;
    }
};

#endif // RENDERENGINE_H
