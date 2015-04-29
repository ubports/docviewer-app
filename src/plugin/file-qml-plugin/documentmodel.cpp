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
#include "fswatcher.h"

#include <QStandardPaths>
#include <QDirIterator>

#include <QDir>
#include <QFileInfo>

#include <QMimeDatabase>
#include <QDateTime>

#include <QDebug>

DocumentModel::DocumentModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_docsMonitor = new FSWatcher;

    connect(m_docsMonitor, SIGNAL(fileAdded(QString)), this,
            SLOT(q_fileAdded(QString)));
    connect(m_docsMonitor, SIGNAL(fileRemoved(QString)), this,
            SLOT(q_fileRemoved(QString)));
    connect(m_docsMonitor, SIGNAL(fileModified(QString)), this,
            SLOT(q_fileModified(QString)));

    setWatchedDirs();
}

void DocumentModel::q_fileRemoved(const QString & path)
{
    for (int i=0; i<m_docs.length(); i++) {
        if (m_docs.at(i).path == path) {
            removeDocumentEntry(i);
            break;
        }
    }
}

void DocumentModel::q_fileAdded(const QString & path)
{
    if (isFileSupported(path)) {
        addDocumentEntry(createEntry(path));
    }
}

void DocumentModel::q_fileModified(const QString & path)
{
    for (int i=0; i<m_docs.length(); i++) {
        if (m_docs.at(i).path == path) {
            m_docs[i] = createEntry(path);

            break;
        }
    }
}

DocumentItem DocumentModel::createEntry(const QString &path)
{
    DocumentItem item;

    QDateTime now = QDateTime::currentDateTime();
    //qDebug() << "Current date is:" << now.toString("dd-MM-yyyy");

    QFileInfo file(path);
    QMimeDatabase db;

    QDateTime lastAccess = file.lastRead();

    item.name = file.fileName();
    item.path = file.absoluteFilePath();
    item.mimetype = db.mimeTypeForFile(path).name();
    item.date = lastAccess.toMSecsSinceEpoch();
    item.size = file.size();

    qint64 dateDiff = lastAccess.daysTo(now);
    if (dateDiff == 0)
        item.dateDiff = DateDiffEnums::Today;

    else if (dateDiff == 1)
        item.dateDiff = DateDiffEnums::Yesterday;

    else if (dateDiff < 7)
        item.dateDiff = DateDiffEnums::LastWeek;

    else if (dateDiff < 30)
        item.dateDiff = DateDiffEnums::LastMonth;

    else
        item.dateDiff = DateDiffEnums::Earlier;

    //qDebug() << "Item" << item.name << "Date:" << item.date;
    //qDebug() << "Item" << item.name << "Item date is:" << lastAccess.toString("dd-MM-yyyy") << "diff is" << dateDiff << "DateDiff is:" << item.dateDiff;

    return item;
}

bool DocumentModel::isFileSupported(const QString &path)
{
    QMimeDatabase db;
    QString mimetype = db.mimeTypeForFile(path).name();

    return (mimetype.startsWith("text/") || mimetype == "application/pdf");
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
    beginRemoveRows(QModelIndex(), index, index);
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

bool DocumentModel::rm(QString path)
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

void DocumentModel::setCustomDir(QString path)
{
    if (m_customDir != path) {
        m_customDir = path;

        setWatchedDirs();
        Q_EMIT customDirChanged();
    }
}

void DocumentModel::setWatchedDirs()
{
    // Clear old watched paths and document list
    m_docsMonitor->clear();
    m_docs.clear();

    if (!m_customDir.isEmpty())
        m_docsMonitor->addDirectory(m_customDir);
    else
        m_docsMonitor->addDirectory(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
}

DocumentModel::~DocumentModel()
{
    delete m_docsMonitor;
}
