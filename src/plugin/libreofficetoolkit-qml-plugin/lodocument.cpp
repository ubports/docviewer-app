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
 * Authors: Anthony Granger <grangeranthony@gmail.com>
 *          Stefano Verzegnassi <stefano92.100@gmail.com>
 */

#include "lodocument.h"
#include "loimageprovider.h"

#include <QDebug>
#include <QQmlEngine>
#include <QQmlContext>
#include <QThread>

#define LOK_USE_UNSTABLE_API
#include <LibreOfficeKit/LibreOfficeKitInit.h>
#include <LibreOfficeKit/LibreOfficeKit.hxx>

// This is the hardcoded Ubuntu/Debian paths to find the LibreOffice
// installation. If you want to use a parallel installation, change the path
// in the following line.
#define LO_PATH "/usr/lib/libreoffice/program/"

LODocument::LODocument(QAbstractListModel *parent):
    QAbstractListModel(parent)
  , m_path("")
  , m_document(nullptr)
{
}

QHash<int, QByteArray> LODocument::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[WidthRole] = "width";
    roles[HeightRole] = "height";
    return roles;
}

int LODocument::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent)
    return m_pages.count();
}

QVariant LODocument::data(const QModelIndex & index, int role) const
{
    if (index.row() < 0 || index.row() > m_pages.count())
        return QVariant();

    const LOItem &loItem = m_pages.at(index.row());

    switch (role) {
    case WidthRole:
        return loItem.width;
    case HeightRole:
        return loItem.height;
    default:
        return 0;
    }
}

void LODocument::setPath(QString &pathName)
{
    if (pathName.isEmpty())
        return;

    m_path = pathName;
    Q_EMIT pathChanged();

    if (!loadDocument(m_path))
        return;

    loadPages();
    loadProvider();
}

bool LODocument::loadDocument(QString &pathName)
{
    qDebug() << "Loading document...";

    if (pathName.isEmpty()) {
        qDebug() << "Can't load the document, path is empty.";
        return false;
    }

    m_office = lok::lok_cpp_init(LO_PATH);
    m_document = m_office->documentLoad(m_path.toUtf8().constData());

    qDebug() << "Document loaded successfully !";

    return true;
}

bool LODocument::loadPages()
{
    qDebug() << "Populating model...";

    m_pages.clear();

    if (!m_document)
        return false;

    // TODO: At the moment this is just hardcoded.
    // LibreOfficeKit returns the whole document, and works in a different way
    // than the poppler-qml-plugin, which is the source of this code.
    // We should use a QQuickItem, divide its surface into a number of tiles,
    // and paint only the ones that becomes visible.
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    LOItem page;
    long docWidth(0);
    long docHeight(0);

    m_document->getDocumentSize(&docWidth, &docHeight);
    page.width = docWidth;
    page.height = docHeight;

    m_pages << page;
    endInsertRows();

    qDebug() << "Model has been successfully populated!";
    Q_EMIT pagesLoaded();

    return true;
}

void LODocument::loadProvider()
{
    QQmlEngine *engine = QQmlEngine::contextForObject(this)->engine();
    engine->addImageProvider(QLatin1String("libreoffice"), new LOImageProvider(this));
}

void LODocument::getDocumentSize(long* pWidth, long* pHeight)
{
    m_document->getDocumentSize(pWidth, pHeight);
}

void LODocument::paintTile(unsigned char *pBuffer, const int nCanvasWidth, const int nCanvasHeight, const int nTilePosX, const int nTilePosY, const int nTileWidth, const int nTileHeight)
{
    m_document->initializeForRendering();
    m_document->paintTile(pBuffer, nCanvasWidth, nCanvasHeight, nTilePosX, nTilePosY, nTileWidth, nTileHeight);
}

LODocument::~LODocument()
{
}
