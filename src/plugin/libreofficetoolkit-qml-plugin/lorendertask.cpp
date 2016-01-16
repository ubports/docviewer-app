#include "lorendertask.h"

bool LoRenderTask::canBeRunInParallel(AbstractRenderTask* prevTask)
{
    Q_ASSERT(prevTask != nullptr);
    if (prevTask->type() == RttTile || prevTask->type() == RttImpressThumbnail) {
        LoRenderTask* loTask = static_cast<LoRenderTask*>(prevTask);

        // Another document or the same part in the same document can be run parallel.
        return (loTask->document() != m_document ||
                loTask->part() == m_part);
    }

    return true;
}

QImage TileRenderTask::doWork()
{
    return m_document->paintTile(m_part, m_area.size(), m_area, m_zoom);
}

QImage ThumbnailRenderTask::doWork()
{
    return m_document->paintThumbnail(m_part, m_size);
}
