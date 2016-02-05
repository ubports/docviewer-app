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

LOPartsImageResponse::LOPartsImageResponse(bool isRequestValid)
    : m_taskId(0)
{
    if (!isRequestValid) {
        m_errorString = "Requested size or id are not valid.";

        QMetaObject::invokeMethod(this, "finished", Qt::QueuedConnection);
        return;
    }

    connect(RenderEngine::instance(), &RenderEngine::taskRenderFinished,
            this, [&](AbstractRenderTask *task, QImage img) {
        if (m_taskId == task->id() && task->type() == RttImpressThumbnail) {
            m_image = img;
            Q_EMIT finished();
        }
    }, Qt::BlockingQueuedConnection);
}

LOPartsImageResponse::~LOPartsImageResponse()
{
    disconnect(this);

    QMetaObject::invokeMethod(RenderEngine::instance(), "dequeueTask",
                              Qt::QueuedConnection,
                              Q_ARG(int, m_taskId));
}

QQuickTextureFactory * LOPartsImageResponse::textureFactory() const
{
    return QQuickTextureFactory::textureFactoryForImage(m_image);
}
