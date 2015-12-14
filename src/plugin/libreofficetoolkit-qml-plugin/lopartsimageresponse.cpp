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
#include "renderengine.h"
#include "rendertask.h"

LOPartsImageResponse::LOPartsImageResponse(const QSharedPointer<LODocument>& document, const QString & id, const QSize & requestedSize) :
    m_document(document),
    m_requestedId(id),
    m_requestedSize(requestedSize)
{
    QString type = m_requestedId.section("/", 0, 0);

    if (m_requestedSize.isNull() || type != "part" ||
            m_document->documentType() != LODocument::PresentationDocument) {
        m_errorString = "Requested size or id are not valid.";
    } else {
        m_renderEngineRequestedId = RenderEngine::getNextId();

        auto task = new ThumbnailRenderTask();
        task->setId(m_renderEngineRequestedId);
        task->setPart(m_requestedId.section("/", 1, 1).toInt());
        task->setDocument(m_document);

        // TODO: use QSize
        task->setSize(qreal(256.0));

        connect(RenderEngine::instance(), &RenderEngine::taskRenderFinished,
                this, &LOPartsImageResponse::slotTaskRenderFinished);

        RenderEngine::instance()->enqueueTask(task);
    }
}

QString LOPartsImageResponse::errorString() const
{
    return m_errorString;
}

QQuickTextureFactory * LOPartsImageResponse::textureFactory() const
{
    return QQuickTextureFactory::textureFactoryForImage(m_image);
}


void LOPartsImageResponse::slotTaskRenderFinished(AbstractRenderTask* task, QImage img)
{
    int id = task->id();

    if (m_renderEngineRequestedId == id &&
            task->type() == RttImpressThumbnail) {
        m_image = img;
        Q_EMIT finished();
    }
}
