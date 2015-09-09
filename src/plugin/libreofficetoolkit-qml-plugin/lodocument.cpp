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
 */

#include "lodocument.h"
#include "twips.h"
#include "config.h"

#include <QImage>
#include <QDebug>

#define LOK_USE_UNSTABLE_API
#include <LibreOfficeKit/LibreOfficeKitInit.h>
#include <LibreOfficeKit/LibreOfficeKit.hxx>

// TODO: Error management

lok::Office *LODocument::s_office = nullptr;

LODocument::LODocument()
  : m_path("")
  , m_document(nullptr)
{
    // This space is intentionally empty.
}

// Return the path of the loaded document
QString LODocument::path() const
{
    return m_path;
}

// Set the path of the document, then it tries to load it.
void LODocument::setPath(QString &pathName)
{
    if (pathName.isEmpty())
        return;

    m_path = pathName;
    Q_EMIT pathChanged();

    // Load the new document
    this->loadDocument(m_path);
}

// Load the document
bool LODocument::loadDocument(QString &pathName)
{
    qDebug() << "Loading document...";

    if (pathName.isEmpty()) {
        qDebug() << "Can't load the document, path is empty.";
        return false;
    }

    if (!s_office)
        s_office = lok::lok_cpp_init(getLibreOfficePath());

    m_document = s_office->documentLoad(m_path.toUtf8().constData());

    m_docType = DocumentType(m_document->getDocumentType());
    Q_EMIT documentTypeChanged();

    m_document->initializeForRendering();
    qDebug() << "Document loaded successfully !";

    return true;
}

// Return the type of the loaded document (e.g. text document,
// spreadsheet or presentation).
LODocument::DocumentType LODocument::documentType() const
{
    return m_docType;
}

// Return the size of the document, in TWIPs
QSize LODocument::documentSize() const
{
    if (!m_document)
        return QSize(0, 0);

    long pWidth(0);
    long pHeight(0);
    m_document->getDocumentSize(&pWidth, &pHeight);

    return QSize(pWidth, pHeight);
}

// Paint a tile, with size=canvasSize, of the part of the document defined by
// the rect tileSize.
QImage LODocument::paintTile(QSize canvasSize, QRect tileSize)
{
    QImage result = QImage(canvasSize.width(), canvasSize.height(),  QImage::Format_RGB32);
    m_document->paintTile(result.bits(),
                          canvasSize.width(), canvasSize.height(),
                          Twips::convertPixelsToTwips(tileSize.x()),
                          Twips::convertPixelsToTwips(tileSize.y()),
                          Twips::convertPixelsToTwips(tileSize.width()),
                          Twips::convertPixelsToTwips(tileSize.height()));

    return result.rgbSwapped();
}

/* Export the file in a given format:
 *  - url is a mandatory argument.
 *  - format is optional. If not specified, lok will try to get it from the file
 *    extension given at the 'url' argument.
 *  - filerOptions is also optional.
 */
// TODO: Is there some documentation on safe formats or filterOptions that can
// be used?
bool LODocument::saveAs(QString url, QString format = QString(), QString filterOptions = QString())
{
    if (!m_document) {
        qDebug() << "No loaded document. It's not possible to save this file.";
        return false;
    }

    return m_document->saveAs(url.toLatin1().constData(),
                              format.toLatin1().constData(),
                              filterOptions.toLatin1().constData());
}

LODocument::~LODocument()
{
    delete m_document;
}
