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
#include <pdfPage.h>

typedef QList<Poppler::Page*> PdfPagesList;

class PdfDocument : public QAbstractListModel
{
    Q_OBJECT
    Q_DISABLE_COPY(PdfDocument)

public:
    enum Roles {
        WidthRole = Qt::UserRole + 1,
        HeightRole
    };

    explicit PdfDocument(QAbstractListModel *parent = 0);
    virtual ~PdfDocument();

    QHash<int, QByteArray> roleNames() const;

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    Q_PROPERTY(QString path READ getPath WRITE setPath NOTIFY pathChanged)
    void setPath(QString &pathName);
    QString getPath() const { return path; }

    QString path;

    Q_INVOKABLE QDateTime getDocumentDate(QString data);
    Q_INVOKABLE QString getDocumentInfo(QString data);

private:
    int loadDocument(QString &pathNAme);
    int loadProvider();
    int loadPages();

    Poppler::Document *document;
    QList<PdfPage> m_pages;

private slots:
    void populate(PdfPagesList pagesList);

signals:
    void pathChanged(const QString& newPath);
    void error(const QString& errorMessage);
    void pagesLoaded();
};

#endif // PDFDOCUMENT_H
