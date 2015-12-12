#ifndef LORENDERTASK_H
#define LORENDERTASK_H

#include <QObject>
#include <QImage>
#include <QSharedPointer>
#include <QHash>
#include <QQueue>
#include <QAtomicInt>

#include "../../app/rendertask.h"
#include "lodocument.h"


class LoRenderTask : public AbstractRenderTask
{
public:
    virtual bool canBeRunInParallel(AbstractRenderTask* prevTask);
    virtual void prepare() { m_document->setDocumentPart(m_part); }

    int part() { return m_part; }
    void setPart(int p) { m_part = p; }
    QSharedPointer<LODocument> document() { return m_document; }
    void setDocument(QSharedPointer<LODocument> d) { m_document = d; }
protected:
    int m_part;
    QSharedPointer<LODocument> m_document;
};

class TileRenderTask : public LoRenderTask
{
public:
    virtual RenderTaskType type() { return RttTile; }
    virtual QImage doWork();

    QRect area() { return m_area; }
    void setArea(const QRect& a) { m_area = a; }
    qreal zoom() { return m_zoom; }
    void setZoom(qreal z) { m_zoom = z; }
protected:
    QRect m_area;
    qreal m_zoom;
};

class ThumbnailRenderTask : public LoRenderTask
{
public:
    virtual RenderTaskType type() { return RttImpressThumbnail; }
    virtual QImage doWork();

    qreal size() { return m_size; }
    void setSize(qreal s) { m_size = s; }
protected:
    qreal m_size;
};

#endif // LORENDERTASK_H
