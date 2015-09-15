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
#include "config.h"
#include "twips.h"

#include <QDebug>

LOPartsImageProvider::LOPartsImageProvider(LODocument *document)
    : QQuickImageProvider(QQuickImageProvider::Image, QQuickImageProvider::ForceAsynchronousImageLoading)
{
    m_document = document;
}

QImage LOPartsImageProvider::requestImage(const QString & id, QSize * size, const QSize & requestedSize)
{
    Q_UNUSED(size)
    Q_UNUSED(requestedSize)

    // Here's the tricky magic. For getting a thumbnail of a document part
    // (e.g. a specific slide in a Impress document), we need to change the
    // current active part in LODocument, render the thumbnail, then re-set
    // the previous value through lok::Document::setPath(index).

    QString type = id.section("/", 0, 0);
    QImage result;

    if (type == "part")
    {
        int partNumber = id.section("/", 1, 1).toInt();
        QSize partSize;
        QSize resultSize;

        // Get the current part index and set the index of the part to be rendered.
        int currentPart = m_document->swapCurrentPart(partNumber);

        // Get the size of the part
        partSize = m_document->documentSize();
        partSize.setHeight(Twips::convertTwipsToPixels(partSize.height()));
        partSize.setWidth(Twips::convertTwipsToPixels(partSize.width()));

        // Set the size of the rendered thumbnail
        if (partSize.width() > partSize.height()) {
            resultSize.setWidth(TILE_SIZE);
            resultSize.setHeight(TILE_SIZE * partSize.height() / partSize.width());
        } else {
            resultSize.setHeight(TILE_SIZE);
            resultSize.setWidth(TILE_SIZE * partSize.width() / partSize.height());
        }

        // Render the part to QImage
        result = m_document->paintTile(resultSize, QRect(QPoint(0, 0), partSize));

        // Re-set the earlier current part
        m_document->swapCurrentPart(currentPart);
    }

    return result;


    // Requested size is 0, so return a null image.
  //  return QImage();
}
