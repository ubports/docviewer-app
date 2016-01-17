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

    m_task = new ThumbnailRenderTask();
    m_task->setIsOwnedByCaller(true);   // Take the ownership of the task, for now.
    m_task->setId(RenderEngine::getNextId());
    m_task->setPart(id.section("/", 1, 1).toInt());
    m_task->setDocument(m_document);

    if (!requestedSize.isEmpty()) {
        m_task->setSize(requestedSize);
    } else {
        m_task->setSize(QSize(256, 256));
    }

    connect(RenderEngine::instance(), &RenderEngine::taskRenderFinished,
            this, &LOPartsImageResponse::slotTaskRenderFinished);

    QMetaObject::invokeMethod(RenderEngine::instance(), "enqueueTask",
                              Qt::QueuedConnection,
                              Q_ARG(AbstractRenderTask*, m_task));
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
    disconnect(RenderEngine::instance(), &RenderEngine::taskRenderFinished,
               this, &LOPartsImageResponse::slotTaskRenderFinished);

    if (m_task) {
        // Give to the RenderEngine the ownership of the task, so that it can
        // be dequeued and deleted.
        m_task->setIsOwnedByCaller(false);

        QMetaObject::invokeMethod(RenderEngine::instance(), "dequeueTask",
                                  Qt::QueuedConnection,
                                  Q_ARG(int, m_task->id()));
    }
}

void LOPartsImageResponse::slotTaskRenderFinished(AbstractRenderTask* task, QImage img)
{
    if (task->id() == m_task->id() && task->type() == RttImpressThumbnail) {
        m_image = img;
        Q_EMIT finished();
    }
}
