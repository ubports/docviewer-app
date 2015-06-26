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
 *         Stefano Verzegnassi <stefano92.100@gmail.com
 */

#include "loimageprovider.h"
#include "lodocument.h"
#include <QQuickImageProvider>
#include <QDebug>

// For benchmarking
#include <QElapsedTimer>

LOImageProvider::LOImageProvider(LODocument *document)
    : QQuickImageProvider(QQuickImageProvider::Image, QQuickImageProvider::ForceAsynchronousImageLoading)
{
    m_document = document;
}

QImage LOImageProvider::requestImage(const QString & id, QSize * size, const QSize & requestedSize)
{
    Q_UNUSED(size)

    // If the requestedSize.width is 0, avoid Poppler rendering
    if (requestedSize.width() > 0) {
        QString type = id.section("/", 0, 0);
        QImage result;

        if (type == "page")
        {
            // Benchmark
            QElapsedTimer benchmarkTimer;
            benchmarkTimer.start();

            // Not used at the moment, since LibreOfficeKit renders the whole document as a single.
            int numPage = id.section("/", 1, 1).toInt();

            long docWidth(0);
            long docHeight(0);
            m_document->getDocumentSize(&docWidth, &docHeight);

            const int requestedWidth = requestedSize.width();
            const int requestedHeight = requestedSize.width() * float(docHeight) / float(docWidth);

            qDebug() << "Document size:" << docWidth << "x" << docHeight;
            qDebug() << "Requested size:" << requestedWidth << "x" << requestedHeight;

            result = QImage(requestedWidth, requestedHeight, QImage::Format_ARGB32);
            m_document->paintTile(result.bits(), requestedWidth, requestedHeight, 0, 0, docWidth, docHeight);

            qDebug() << "Rendering time:" << benchmarkTimer.elapsed() << "msec";
        }

        return result;
    }

    // Requested size is 0, so return a null image.
    return QImage();
}
