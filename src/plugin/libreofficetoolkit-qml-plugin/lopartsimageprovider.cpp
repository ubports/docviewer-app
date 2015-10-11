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

LOPartsImageProvider::LOPartsImageProvider(const QSharedPointer<LODocument>& d)
    : QQuickImageProvider(QQuickImageProvider::Image),
      m_document(d)
{ }

QImage LOPartsImageProvider::requestImage(const QString & id, QSize * size, const QSize & requestedSize)
{
    Q_UNUSED(size)

    QString type = id.section("/", 0, 0);

    if (requestedSize.isNull() || type != "part" ||
            m_document->documentType() != LODocument::PresentationDocument)
        return QImage();

    // Get info from "id".
    int partNumber = id.section("/", 1, 1).toInt();
    int itemId = id.section("/", 2, 2).toInt();

    // Once rendered images can be found in hash.
    if (m_images.contains(itemId))
        return m_images[itemId];

    const int defaultSize = 256;

    RenderEngine::instance()->enqueueTask(m_document, partNumber, defaultSize, itemId);

    // Return default image (empty).
    static QImage defaultImage;
    if (defaultImage.isNull())
        defaultImage = QImage(defaultSize, defaultSize, QImage::Format_ARGB32);

    return defaultImage;
}
