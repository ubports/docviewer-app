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
 *         Stefano Verzegnassi <stefano92.100@gmail.com>
 */

#ifndef PDFDOCUMENT_H
#define PDFDOCUMENT_H

#include <QAbstractListModel>
#include <poppler/qt5/poppler-qt5.h>
#include "pdfitem.h"

typedef QList<Poppler::Page*> PdfPagesList;

class PdfDocument : public QAbstractListModel
{
    Q_OBJECT
    Q_DISABLE_COPY(PdfDocument)
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)

public:
    enum Roles {
        WidthRole = Qt::UserRole + 1,
        HeightRole
    };

    explicit PdfDocument(QAbstractListModel *parent = 0);
    virtual ~PdfDocument();

    QString path() const { return m_path; }
    void setPath(QString &pathName);

    QHash<int, QByteArray> roleNames() const;

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    Q_INVOKABLE QDateTime getDocumentDate(QString data);
    Q_INVOKABLE QString getDocumentInfo(QString data);

Q_SIGNALS:
    void pathChanged();
    void error(const QString& errorMessage);
    void pagesLoaded();

private slots:
    void _q_populate(PdfPagesList pagesList);

private:
    QString m_path;

    bool loadDocument(QString &pathNAme);
    void loadProvider();
    bool loadPages();

    Poppler::Document *m_document;
    QList<PdfItem> m_pages;
};

#endif // PDFDOCUMENT_H
