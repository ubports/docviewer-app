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
    AbstractRenderTask(): m_id(0), m_isOwnedByCaller(false) { }
    virtual RenderTaskType type() { return RttUnknown; }
    virtual QImage doWork() = 0 ;
    virtual ~AbstractRenderTask() { }
    virtual bool canBeRunInParallel(AbstractRenderTask*) { return true; }
    virtual void prepare() = 0 ;

    int id() { return m_id; }
    void setId(int i) { m_id = i; }
    bool isOwnedByCaller() { return m_isOwnedByCaller; }
    void setIsOwnedByCaller(bool v) { m_isOwnedByCaller = v; }
protected:
    int m_id;
    bool m_isOwnedByCaller;
};

Q_DECLARE_METATYPE(AbstractRenderTask*)

#endif // RENDERTASK_H
