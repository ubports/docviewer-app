/*
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

#include "lopartsimageprovider.h"
#include "lopartsimageresponse.h"

#include "lodocument.h"

#include "../../app/renderengine.h"
#include "lorendertask.h"

LOPartsImageProvider::LOPartsImageProvider(const QSharedPointer<LODocument>& d)
    : QQuickAsyncImageProvider()
    , m_document(d)
{ }

QQuickImageResponse *LOPartsImageProvider::requestImageResponse(const QString & id, const QSize & requestedSize)
{
    QString type = id.section("/", 0, 0);
    int part = id.section("/", 1, 1).toInt();
    bool isValid = bool(!requestedSize.isNull() || type == "part");

    auto response = new LOPartsImageResponse(isValid);

    if (isValid) {
        int taskId = RenderEngine::getNextId();
        response->setTaskId(taskId);
        RenderEngine::instance()->enqueueTask(createTask(part, requestedSize, taskId));
    }

    return response;
}

ThumbnailRenderTask* LOPartsImageProvider::createTask(int part, const QSize &size, int id) const
{
    ThumbnailRenderTask* task = new ThumbnailRenderTask();
    task->setId(id);
    task->setPart(part);
    task->setDocument(m_document);
    task->setSize(size.isEmpty() ? QSize(256, 256) : size);

    return task;
}
