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

#include <poppler/qt5/poppler-qt5.h>
#include <QQuickImageProvider>
#include <QDebug>

#include "pdfimageprovider.h"

PdfImageProvider::PdfImageProvider(Poppler::Document *pdfDocument)
    : QQuickImageProvider(QQuickImageProvider::Image, QQuickImageProvider::ForceAsynchronousImageLoading)
{
    this->document = pdfDocument;
}

QImage PdfImageProvider::requestImage(const QString & id, QSize * size, const QSize & requestedSize)
{
    Q_UNUSED(size)

    // If the requestedSize.width is 0, avoid Poppler rendering
    // FIXME: Actually it works correctly, but an error is anyway shown in the application output.
    if (requestedSize.width() > 0) {
        QString type = id.section("/", 0, 0);
        QImage result;
        QSizeF pageSize;
        QSizeF pageSizePhys;
        float res = 0;
        Poppler::Page *page;

        if (type == "page")
        {
            int numPage = id.section("/", 1, 1).toInt();

            // Useful for debugging, keep commented unless you need it.
            //  qDebug() << "Page" << numPage + 1 << "requested";

            page = document->page(numPage);

            pageSize = page->pageSizeF();
            pageSizePhys.setWidth(pageSize.width() / 72);
            res = requestedSize.width() / pageSizePhys.width();

            // Useful for debugging, keep commented unless you need it.
            /*
            qDebug() << "Requested size :" << requestedSize.width() << ";" << requestedSize.height();
            qDebug() << "Size :" << pageSizePhys.width() << ";" << pageSizePhys.height();
            qDebug() << "Resolution :" << res;
            */

            // Render the page to QImage
            result = page->renderToImage(res, res);
        }

        return result;
    }

    // Requested size is 0, so return a null image.
    return QImage();
}
