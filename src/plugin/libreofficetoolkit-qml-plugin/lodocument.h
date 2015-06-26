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

#ifndef LODOCUMENT_H
#define LODOCUMENT_H

#include <QAbstractListModel>

namespace lok {
class Office;
class Document;
}

struct LOItem {
    int width;
    int height;
};

class LODocument : public QAbstractListModel
{
    Q_OBJECT
    Q_DISABLE_COPY(LODocument)
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(DocumentType documentType READ documentType NOTIFY documentTypeChanged)
    Q_ENUMS(DocumentType)

public:
    enum Roles {
        WidthRole = Qt::UserRole + 1,
        HeightRole
    };

    enum DocumentType {
        TextDocument = 0,
        SpreadsheetDocument = 1,
        PresentationDocument = 2,
        DrawingDocument = 3,
        OtherDocument = 4
    };

    explicit LODocument(QAbstractListModel *parent = 0);
    virtual ~LODocument();

    QString path() const { return m_path; }
    void setPath(QString &pathName);

    DocumentType documentType() const { return m_docType; }

    QHash<int, QByteArray> roleNames() const;

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    void getDocumentSize(long* pWidth, long* pHeight);
    void paintTile(unsigned char* pBuffer,  const int nCanvasWidth,
                   const int nCanvasHeight, const int nTilePosX,
                   const int nTilePosY,     const int nTileWidth,
                   const int nTileHeight);

Q_SIGNALS:
    void pathChanged();
    void error(const QString& errorMessage);
    void pagesLoaded();
    void documentTypeChanged();

private:
    QString m_path;
    DocumentType m_docType;

    bool loadDocument(QString &pathNAme);
    void loadProvider();
    bool loadPages();

    void unloadProvider();

    lok::Office *m_office;
    lok::Document *m_document;
    QList<LOItem> m_pages;
};

#endif // LODOCUMENT_H
