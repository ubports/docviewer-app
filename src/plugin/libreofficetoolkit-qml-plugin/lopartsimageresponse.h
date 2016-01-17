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

#ifndef LOPARTSIMAGERESPONSE_H
#define LOPARTSIMAGEPROVIDER_H

// For QQuickImageResponse
#include <qquickimageprovider.h>
#include <QSharedPointer>

class LODocument;
class AbstractRenderTask;
class ThumbnailRenderTask;

class LOPartsImageResponse : public QQuickImageResponse
{
public:
    LOPartsImageResponse(const QSharedPointer<LODocument>& document, const QString & id, const QSize & requestedSize, bool requestIsValid);
    ~LOPartsImageResponse();

    QString errorString() const override;
    QQuickTextureFactory * textureFactory() const override;
    void cancel() override;

private Q_SLOTS:
    void slotTaskRenderFinished(AbstractRenderTask* task, QImage img);

private:
    QSharedPointer<LODocument> m_document;
    ThumbnailRenderTask* m_task;

    QString m_errorString;
    QImage m_image;
};


#endif // LOPARTSIMAGERESPONSE_H
