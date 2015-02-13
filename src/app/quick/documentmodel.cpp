/*
 *  Copyright (C) 2014, 2015
 *                  Stefano Verzegnassi <stefano92.100@gmail.com>
 *
 *  This is free software: you can redistribute it and/or modify
 *  it under the terms of the  GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License.
 *
 *  This is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see http://www.gnu.org/licenses/.
 */

#include "documentmodel.h"

#include <QMimeDatabase>
#include <QDateTime>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

DocumentModel::DocumentModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_docsMonitor = new QFileSystemWatcher;

    connect(m_docsMonitor, SIGNAL(directoryChanged(QString)), this,
            SLOT(_q_directoryChanged(QString)));

    m_docsMonitor->addPath(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));

    Q_FOREACH (const QString dir, m_docsMonitor->directories()) {
        parseDirectoryContent(dir);
    }
}

void DocumentModel::_q_directoryChanged(QString path)
{
    QMutableListIterator<DocumentItem> i(m_docs);

    while (i.hasNext()) {
        if (i.next().path.startsWith(path)) {
            beginRemoveRows(QModelIndex(), rowCount(), rowCount());
            i.remove();
            endRemoveRows();
        }
    }

    parseDirectoryContent(path);
}

void DocumentModel::parseDirectoryContent(QString path)
{
    QDirIterator dir(path, QDir::Files | QDir::NoDotAndDotDot | QDir::Readable,
                     QDirIterator::Subdirectories);

    while (dir.hasNext()) {
        dir.next();

        QDateTime now = QDateTime::currentDateTime();
        //qDebug() << "Current date is:" << now.toString("dd-MM-yyyy");

        QMimeDatabase db;
        QString mimetype = db.mimeTypeForFile(dir.filePath()).name();

        if (mimetype.startsWith("text/") || mimetype == "application/pdf") {
            QFileInfo file(dir.filePath());
            DocumentItem item;

            // FIXME: We'd like to track the date-time of the last access.
            // This seems not to be possible to do with QFileInfo,
            // since it return the date-time of the last time we watched
            // for file infos using this class.
            // We could track instead the last time doc-viewer has opened
            // the file, but we need to use a database.
            QDateTime lastAccess = file.lastRead();

            item.name = file.fileName();
            item.path = file.absoluteFilePath();
            item.mimetype = mimetype;
            item.date = lastAccess.toMSecsSinceEpoch();
            item.size = file.size();

            qint64 dateDiff = lastAccess.daysTo(now);
            if (dateDiff == 0) {
                item.dateDiff = 0;
            }
            else if (dateDiff == 1) {
                item.dateDiff = 1;
            }
            else if (dateDiff < 7) {
                item.dateDiff = 2;
            }
            else if (dateDiff < 30) {
                item.dateDiff = 3;
            }
            else {
                item.dateDiff = 4;
            }
            //qDebug() << "Item" << item.name << "Date:" << item.date;
            //qDebug() << "Item" << item.name << "Item date is:" << lastAccess.toString("dd-MM-yyyy") << "diff is" << dateDiff << "DateDiff is:" << item.dateDiff;

            addDocumentEntry(item);
        }
    }
}

QHash<int, QByteArray> DocumentModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[PathRole] = "path";
    roles[NameRole] = "name";
    roles[MimetypeRole] = "mimetype";
    roles[DateRole] = "date";
    roles[DateDiffRole] = "dateDiff";
    roles[SizeRole] = "size";

    return roles;
}

void DocumentModel::addDocumentEntry(DocumentItem item)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_docs.append(item);
    endInsertRows();
}

void DocumentModel::removeDocumentEntry(int index)
{
    beginRemoveRows(QModelIndex(), rowCount(), rowCount());
    m_docs.removeAt(index);
    endRemoveRows();
}

int DocumentModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent)
    return m_docs.count();
}

QVariant DocumentModel::data(const QModelIndex & index, int role) const
{
    if (index.row() < 0 || index.row() > m_docs.count())
        return QVariant();

    const DocumentItem &item = m_docs.at(index.row());

    switch (role) {
    case PathRole:
        return item.path;
    case NameRole:
        return item.name;
    case MimetypeRole:
        return item.mimetype;
    case DateRole:
        return item.date;
    case DateDiffRole:
        return item.dateDiff;
    case SizeRole:
        return item.size;
    default:
        return 0;
    }
}

DocumentModel::~DocumentModel()
{
    delete m_docsMonitor;
}

SortFilterDocumentModel::SortFilterDocumentModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    m_model = new DocumentModel();
    setSourceModel(m_model);

    setSortRole(DocumentModel::DateRole);
    setSortCaseSensitivity(Qt::CaseInsensitive);
    setSortLocaleAware(true);

    sort(0, Qt::DescendingOrder);
    setDynamicSortFilter(true);
}

SortFilterDocumentModel::~SortFilterDocumentModel()
{
    delete m_model;
}

int SortFilterDocumentModel::rowCount()
{
    return m_model->rowCount();
}

bool SortFilterDocumentModel::rm(QString path)
{
    bool result = false;
    QDir dir(path);

    if (dir.exists()) {
        result = dir.removeRecursively();
    } else {
        QFile fi(path);
        result = fi.remove();
    }

    return result;
}
