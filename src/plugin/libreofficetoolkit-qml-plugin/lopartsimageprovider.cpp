/*
 * Copyright (C) 2015 Canonical, Ltd.
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
#include "lodocument.h"
#include "renderengine.h"

LOPartsImageProvider::LOPartsImageProvider(LODocument *document)
    : QQuickImageProvider(QQuickImageProvider::Image, QQuickImageProvider::ForceAsynchronousImageLoading)
    , m_document(document)
{ }

QImage LOPartsImageProvider::requestImage(const QString & id, QSize * size, const QSize & requestedSize)
{
    Q_UNUSED(size)

    QString type = id.section("/", 0, 0);

    if (requestedSize.isNull() || type != "part" ||
            m_document->documentType() != LODocument::PresentationDocument)
        return QImage();

    // Wait for any in-progress rendering to be completed
    while (RenderEngine::instance()->activeTaskCount() != 0) { }

    // Lock the render engine
    RenderEngine::instance()->setEnabled(false);

    // Render the part to QImage
    int partNumber = id.section("/", 1, 1).toInt();
    QImage result = m_document->paintThumbnail(partNumber, 256.0);

    // Unlock the render engine
    RenderEngine::instance()->setEnabled(true);

    return result;
}
