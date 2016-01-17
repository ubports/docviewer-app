/*
 * Copyright (C) 2015 Roman Shchekin
 * Copyright (C) 2015 Stefano Verzegnassi
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3, as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "lopartsimageresponse.h"
#include "lodocument.h"
#include "../../app/renderengine.h"
#include "lorendertask.h"

LOPartsImageResponse::LOPartsImageResponse(const QSharedPointer<LODocument>& document, const QString & id, const QSize & requestedSize, bool requestIsValid) :
    m_document(document)
{
    if (!requestIsValid) {
        m_errorString = "Requested size or id are not valid.";

        // Emit 'finished' signal and return an empty image.
        QMetaObject::invokeMethod(this, "finished", Qt::QueuedConnection);
        return;
    }

    m_taskId = RenderEngine::getNextId();

    auto task = new ThumbnailRenderTask();
    task->setId(m_taskId);
    task->setPart(id.section("/", 1, 1).toInt());
    task->setDocument(m_document);

    if (!requestedSize.isEmpty()) {
        task->setSize(requestedSize);
    } else {
        task->setSize(QSize(256, 256));
    }

    // Use Qt::BlockingQueuedConnection type in order to avoid that the returned
    // AbstractRenderTask* pointer is deleted by RenderEngine before we
    // finished to handle it.
    connect(RenderEngine::instance(), &RenderEngine::taskRenderFinished,
            this, &LOPartsImageResponse::slotTaskRenderFinished, Qt::BlockingQueuedConnection);

    RenderEngine::instance()->enqueueTask(task);
}

LOPartsImageResponse::~LOPartsImageResponse()
{
    this->cancel();
}

QString LOPartsImageResponse::errorString() const
{
    return m_errorString;
}

QQuickTextureFactory * LOPartsImageResponse::textureFactory() const
{
    return QQuickTextureFactory::textureFactoryForImage(m_image);
}

void LOPartsImageResponse::cancel()
{
    disconnect(this);

    // Remove task from the queue, if it's still waiting for being rendered.
    if (m_taskId) {
        RenderEngine::instance()->dequeueTask(m_taskId);
    }
}

void LOPartsImageResponse::slotTaskRenderFinished(AbstractRenderTask* task, QImage img)
{
    if (task->id() == m_taskId && task->type() == RttImpressThumbnail) {
        m_image = img;
        Q_EMIT finished();
    }
}
