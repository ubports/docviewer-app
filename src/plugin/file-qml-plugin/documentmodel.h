/*
  Copyright (C) 2014, 2015 Stefano Verzegnassi <stefano92.100@gmail.com>

    This is free software: you can redistribute it and/or modify
  it under the terms of the  GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License.

    This is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
  along with this program. If not, see http://www.gnu.org/licenses/.
*/

#ifndef DOCUMENTMODEL_H
#define DOCUMENTMODEL_H

#include <QAbstractListModel>
#include <QSortFilterProxyModel>

#include <QStandardPaths>
#include <QFileSystemWatcher>
#include <QDirIterator>

struct DocumentItem {
    QString name;
    QString path;
    QString mimetype;
    qint64 date;
    int dateDiff;
    qint64 size;
};

class DocumentModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString customDir READ getCustomDir WRITE setCustomDir NOTIFY customDirChanged)

public:
    enum Roles {
        NameRole = Qt::UserRole + 1,
        PathRole,
        MimetypeRole,
        DateRole,
        DateDiffRole,
        SizeRole
    };

    DocumentModel(QObject *parent = 0);
    ~DocumentModel();

    QHash<int, QByteArray> roleNames() const;

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    QString getCustomDir() const { return m_customDir; }

    Q_INVOKABLE bool rm(QString path);

    void parseDirectoryContent(QString path);

signals:
    void customDirChanged();

public Q_SLOTS:
    void setCustomDir(QString path);

private Q_SLOTS:
    void _q_directoryChanged(QString path);

private:   
    void setWatchedDirs();
    void addDocumentEntry(DocumentItem item);
    void removeDocumentEntry(int index);

    QList<DocumentItem> m_docs;
    QFileSystemWatcher *m_docsMonitor;
    QString m_customDir;
};

#endif // DOCUMENTMODEL_H
