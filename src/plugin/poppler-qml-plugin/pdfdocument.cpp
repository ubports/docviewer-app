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

#include "pdfdocument.h"
#include "pdfimageprovider.h"

#include <poppler/qt5/poppler-qt5.h>
#include <QDebug>
#include <QQmlEngine>
#include <QQmlContext>

#include <QtConcurrent/QtConcurrent>

PdfDocument::PdfDocument(QAbstractListModel *parent):
    QAbstractListModel(parent)
  , m_path("")
  , m_providersNumber(1)
  , m_tocModel(nullptr)
{
    qRegisterMetaType<PdfPagesList>("PdfPagesList");
}

QHash<int, QByteArray> PdfDocument::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[WidthRole] = "width";
    roles[HeightRole] = "height";
    return roles;
}

int PdfDocument::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent)
    return m_pages.count();
}

QVariant PdfDocument::data(const QModelIndex & index, int role) const
{
    if (index.row() < 0 || index.row() > m_pages.count())
        return QVariant();

    const PdfItem &pdfItem = m_pages.at(index.row());

    switch (role) {
    case WidthRole:
        return pdfItem.width();
    case HeightRole:
        return pdfItem.height();
    default:
        return 0;
    }
}

void PdfDocument::setPath(QString &pathName)
{
    if (pathName.isEmpty())
        return;

    m_path = pathName;
    Q_EMIT pathChanged();

    if (!loadDocument(m_path))
        return;

    // Init toc model
    m_tocModel = new PdfTocModel;
    m_tocModel->setDocument(m_document);
    Q_EMIT tocModelChanged();

    loadPages();
    loadProvider();
}

bool PdfDocument::loadDocument(QString &pathName)
{
    qDebug() << "Loading document...";

    if (pathName.isEmpty()) {
        qDebug() << "Can't load the document, path is empty.";
        return false;
    }

    m_document = Poppler::Document::load(pathName);

    if (!m_document || m_document->isLocked()) {
        qDebug() << "ERROR : Can't open the document located at " + pathName;
        Q_EMIT error("Can't open the document located at " + pathName);

        delete m_document;
        return false;
    }

    qDebug() << "Document loaded successfully !";

    m_document->setRenderHint(Poppler::Document::Antialiasing, true);
    m_document->setRenderHint(Poppler::Document::TextAntialiasing, true);

    return true;
}

QDateTime PdfDocument::getDocumentDate(QString data)
{
    if (!m_document)
        return QDateTime();

    if (data == "CreationDate" || data == "ModDate")
        return m_document->date(data);
    else
        return QDateTime();
}

QString PdfDocument::getDocumentInfo(QString data)
{
    if (!m_document)
        return QString("");

    if (data == "Title" || data == "Subject" || data == "Author" || data == "Creator" || data == "Producer")
        return m_document->info(data);
    else
        return QString("");
}

bool PdfDocument::loadPages()
{
    qDebug() << "Populating model...";

    m_pages.clear();

    if (!m_document)
        return false;

    Poppler::Document* document = m_document;
    QtConcurrent::run( [=] {
        PdfPagesList pages;

        for( int i = 0; i < document->numPages(); ++i )
            pages.append(document->page(i));

        QMetaObject::invokeMethod(this, "_q_populate", Qt::QueuedConnection, Q_ARG(PdfPagesList, pages));
    });

    return true;
}

void PdfDocument::_q_populate(PdfPagesList pagesList)
{
    qDebug() << "Number of pages:" << pagesList.count();

    Q_FOREACH (Poppler::Page *page, pagesList) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        m_pages << page;
        endInsertRows();
    }

    qDebug() << "Model has been successfully populated!";
    Q_EMIT pagesLoaded();
}

void PdfDocument::loadProvider()
{
    // WORKAROUND: QQuickImageProvider should create multiple threads to load more images at the same time.
    // [QTBUG-37998] QQuickImageProvider can block its separate thread with ForceAsynchronousImageLoading
    // Link: https://bugreports.qt.io/browse/QTBUG-37988

    // WORKAROUND: ARM SoCs can disable some of their cores when the load is not particulary high.
    // This causes a wrong value for the "newProvidersNumber" variable.
    // We hard-code its value to 4 (which is the number of available core on all the supported devices).
//    int newProvidersNumber = QThread::idealThreadCount();
    int newProvidersNumber = 4;

    if (newProvidersNumber != m_providersNumber) {
        m_providersNumber = newProvidersNumber;
        Q_EMIT providersNumberChanged();
    }

    qDebug() << "Ideal number of image providers is:" << m_providersNumber;

    qDebug() << "Loading image provider(s)...";
    QQmlEngine *engine = QQmlEngine::contextForObject(this)->engine();

    for (int i=0; i<m_providersNumber; i++)
        engine->addImageProvider(QLatin1String("poppler" + QByteArray::number(i)), new PdfImageProvider(m_document));

    qDebug() << "Image provider(s) loaded successfully !";
}

PdfDocument::~PdfDocument()
{
}
