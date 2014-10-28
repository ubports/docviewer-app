/*
 * Copyright (C) 2013 Canonical, Ltd.
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

#include <pdfModel.h>
#include <pdfPage.h>
#include <pageImageProvider.h>
#include <pagesWorkerThread.h>

#include <poppler/qt5/poppler-qt5.h>
#include <QDebug>
#include <QQmlEngine>
#include <QQmlContext>

PdfModel::PdfModel(QAbstractListModel *parent):
    QAbstractListModel(parent)
{
    int metatype_id = qRegisterMetaType<PdfPagesList>("PdfPagesList");
}

QHash<int, QByteArray> PdfModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[WidthRole] = "width";
    roles[HeightRole] = "height";
    return roles;
}

int PdfModel::rowCount(const QModelIndex & parent) const
{
    return m_pages.count();
}

QVariant PdfModel::data(const QModelIndex & index, int role) const
{
    if (index.row() < 0 || index.row() > m_pages.count())
        return QVariant();

    const PdfPage &pdfPage = m_pages.at(index.row());

    switch (role) {
    case WidthRole:
        return pdfPage.width();
    case HeightRole:
        return pdfPage.height();
    default:
        return 0;
    }
}

void PdfModel::setPath(QString &pathName)
{
    if (pathName.isEmpty())
    {
        return;
    }

    this->path = pathName;

    emit pathChanged(pathName);

    if ( !loadDocument(pathName) ) {
        return;
    }

    loadPages();
    loadProvider();
}

int PdfModel::loadDocument(QString &pathName)
{
    qDebug() << "Loading document...";

    if (pathName.isEmpty()) {
        qDebug() << "Can't load the document, path is empty.";
        return 0;
    }

    this->document = Poppler::Document::load(pathName);

    if (!document || document->isLocked()) {
        qDebug() << "ERROR : Can't open the document located at " + pathName;
        emit error("Can't open the document located at " + pathName);

        delete document;
        return 0;
    }

    qDebug() << "Document loaded successfully !";

    document->setRenderHint(Poppler::Document::Antialiasing, true);
    document->setRenderHint(Poppler::Document::TextAntialiasing, true);

    return 1;
}

QDateTime PdfModel::getDocumentDate(QString data)
{
    if (!document) {
        return QDateTime();
    }

    if (data == "CreationDate" || data == "ModDate") {
        return document->date(data);
    } else {
        return QDateTime();
    }
}

QString PdfModel::getDocumentInfo(QString data)
{
    if (!document) {
        return QString("");
    }

    if (data == "Title" || data == "Subject" || data == "Author" || data == "Creator" || data == "Producer") {
        return document->info(data);
    } else {
        return QString("");
    }
}

int PdfModel::loadPages()
{
    qDebug() << "Populating model...";

    m_pages.clear();

    if (!document) {
        return 0;
    }

    PDFPagesWorkerThread *workerThread = new PDFPagesWorkerThread();
    workerThread->setDocument(this->document);

    connect(workerThread, SIGNAL(resultReady(PdfPagesList)), this, SLOT(populate(PdfPagesList)));
    connect(workerThread, SIGNAL(finished()), workerThread, SLOT(deleteLater()));
    workerThread->start();

    return 1;
}

void PdfModel::populate(PdfPagesList pagesList)
{
    qDebug() << "Number of pages:" << pagesList.length();

    foreach (Poppler::Page *page, pagesList) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        m_pages << page;
        endInsertRows();
    }

    qDebug() << "Model has been successfully populated!";
    emit pagesLoaded();
}

int PdfModel::loadProvider()
{
    qDebug() << "Loading image provider...";
    QQmlEngine *engine = QQmlEngine::contextForObject(this)->engine();

    engine->addImageProvider(QLatin1String("poppler"), new PageImageProvider(document));

    qDebug() << "Image provider loaded successfully !";

    return 1;
}

PdfModel::~PdfModel()
{
}
