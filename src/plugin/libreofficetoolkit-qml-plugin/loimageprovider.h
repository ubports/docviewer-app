/*
 * Copyright (C) 2013-2015 Canonical, Ltd.
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
 * Author: Anthony Granger <grangeranthony@gmail.com>
 *         Stefano Verzegnassi <stefano92.100@gmail.com>
 */

#ifndef LOIMAGEPROVIDER_H
#define LOIMAGEPROVIDER_H

#include <QQuickImageProvider>

class LODocument;


class LOImageProvider : public QQuickImageProvider
{
public:
    LOImageProvider(LODocument *document);
    QImage requestImage(const QString & id, QSize * size, const QSize & requestedSize);

private:
    LODocument *m_document;
};

#endif // LOIMAGEPROVIDER_H
