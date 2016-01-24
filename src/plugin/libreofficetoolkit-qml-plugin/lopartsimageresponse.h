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

class LOPartsImageResponse : public QQuickImageResponse
{
public:
    LOPartsImageResponse(bool isRequestValid);
    ~LOPartsImageResponse();

    void setTaskId(const int id) { m_taskId = id; }
    QString errorString() const override { return m_errorString; }
    QQuickTextureFactory * textureFactory() const override;

private:
    QString m_errorString;
    QImage m_image;
    int m_taskId;
};


#endif // LOPARTSIMAGERESPONSE_H
