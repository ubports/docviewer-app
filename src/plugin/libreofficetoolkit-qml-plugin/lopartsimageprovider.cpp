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

LOPartsImageProvider::LOPartsImageProvider(const QSharedPointer<LODocument>& d)
    : QQuickAsyncImageProvider(),
      m_document(d)
{ }

QQuickImageResponse *LOPartsImageProvider::requestImageResponse(const QString & id, const QSize & requestedSize)
{
    QString type = id.section("/", 0, 0);

    bool isValid = bool(!requestedSize.isNull() || type == "part" ||
            m_document.data()->documentType() == LODocument::PresentationDocument);

    return new LOPartsImageResponse(m_document, id, requestedSize, isValid);
}
