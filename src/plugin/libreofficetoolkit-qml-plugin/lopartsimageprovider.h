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
 *
 */

#ifndef LOPARTSIMAGEPROVIDER_H
#define LOPARTSIMAGEPROVIDER_H

// For QQuickAsyncImageProvider
#include <qquickimageprovider.h>
#include <QSharedPointer>

class LODocument;
class ThumbnailRenderTask;

class LOPartsImageProvider : public QQuickAsyncImageProvider
{
public:
    LOPartsImageProvider(const QSharedPointer<LODocument>& d);
    QQuickImageResponse* requestImageResponse(const QString & id, const QSize & requestedSize);

private:
    ThumbnailRenderTask* createTask(int part, const QSize &size, int id) const;

private:
    QSharedPointer<LODocument> m_document;
};

#endif // LOPARTSIMAGEPROVIDER_H
