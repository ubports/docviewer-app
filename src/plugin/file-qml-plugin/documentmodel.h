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

    void parseDirectoryContent(QString path);

private Q_SLOTS:
    void _q_directoryChanged(QString path);

private:   
    void addDocumentEntry(DocumentItem item);
    void removeDocumentEntry(int index);

    QList<DocumentItem> m_docs;
    QFileSystemWatcher *m_docsMonitor;
};

class SortFilterDocumentModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:        
    SortFilterDocumentModel(QObject *parent = 0);
    ~SortFilterDocumentModel();

    int rowCount();
    Q_INVOKABLE bool rm(QString path);

Q_SIGNALS:
    void countChanged();

private:
    DocumentModel *m_model;
};

#endif // DOCUMENTMODEL_H
