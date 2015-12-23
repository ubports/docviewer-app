#ifndef RENDERTASK_H
#define RENDERTASK_H

#include <QObject>
#include <QImage>
#include <QSharedPointer>
#include <QHash>
#include <QQueue>

/* Required for super-fast type detection.
 * NOTE: only leaf nodes in inheritance tree have correct types.
 */
enum RenderTaskType
{
    RttUnknown = 0x0,
    RttTile = 0x1,
    RttImpressThumbnail = 0x2,
    RttPdfPage = 0x3
};

class AbstractRenderTask
{
public:
    virtual RenderTaskType type() { return RttUnknown; }
    virtual QImage doWork() = 0 ;
    virtual ~AbstractRenderTask() { }
    virtual bool canBeRunInParallel(AbstractRenderTask*) { return true; }
    virtual void prepare() = 0 ;

    int id() { return m_id; }
    void setId(int i) { m_id = i; }
protected:
    int m_id;
};

Q_DECLARE_METATYPE(AbstractRenderTask*)

#endif // RENDERTASK_H
