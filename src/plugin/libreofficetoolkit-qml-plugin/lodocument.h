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
 */

#ifndef LODOCUMENT_H
#define LODOCUMENT_H

#include <QObject>

#include "loerror.h"

namespace lok {
class Office;
class Document;
}

class LODocument : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(LODocument)

    Q_PROPERTY(QString                 path         READ path         WRITE setPath         NOTIFY pathChanged)
    Q_PROPERTY(int                     currentPart  READ currentPart  WRITE setCurrentPart  NOTIFY currentPartChanged)
    // Declare partsCount as constant at the moment, since LOK-plugin is just a viewer for now.
    Q_PROPERTY(int                     partsCount   READ partsCount                                                    CONSTANT)
    Q_PROPERTY(int                     documentPart READ documentPart WRITE setDocumentPart NOTIFY documentPartChanged)
    Q_PROPERTY(DocumentType            documentType READ documentType                       NOTIFY documentTypeChanged)
    Q_PROPERTY(LibreOfficeError::Error error        READ error                              NOTIFY errorChanged)
    Q_ENUMS(DocumentType)

public:
    explicit LODocument();
    virtual ~LODocument();

    enum DocumentType {
        TextDocument = 0,
        SpreadsheetDocument = 1,
        PresentationDocument = 2,
        DrawingDocument = 3,
        OtherDocument = 4
    };

    QString path() const;
    void setPath(const QString& pathName);

    int currentPart();
    void setCurrentPart(int index);

    DocumentType documentType() const;

    int documentPart() const;
    void setDocumentPart(int p);

    QSize documentSize() const;

    QImage paintTile(const QSize& canvasSize, const QRect& tileSize, const qreal& zoom = 1.0);
    QImage paintThumbnail(qreal size);

    int partsCount();
    QString getPartName(int index) const;
    void setPart(int index);

    LibreOfficeError::Error error() const;

    Q_INVOKABLE bool saveAs(QString url, QString format, QString filterOptions);

Q_SIGNALS:
    void pathChanged();
    void currentPartChanged();
    void documentTypeChanged();
    void documentPartChanged();
    void errorChanged();

private:
    QString m_path;
    int m_currentPart;
    DocumentType m_docType;
    LibreOfficeError::Error m_error;

    void loadDocument(const QString &pathNAme);

    void setError(const LibreOfficeError::Error &error);

    lok::Document *m_document;

    static lok::Office *s_office;
};

#endif // LODOCUMENT_H
