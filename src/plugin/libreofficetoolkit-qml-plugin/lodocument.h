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

namespace lok {
class Office;
class Document;
}

class LODocument : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(LODocument)

    Q_PROPERTY(QString      path         READ path         WRITE setPath        NOTIFY pathChanged)
    Q_PROPERTY(int          currentPart  READ currentPart  WRITE setCurrentPart NOTIFY currentPartChanged)
    // Declare partsCount as constant at the moment, since LOK-plugin is just a viewer for now.
    Q_PROPERTY(int          partsCount   READ partsCount                                                    CONSTANT)
    Q_PROPERTY(DocumentType documentType READ documentType                      NOTIFY documentTypeChanged)
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
    void setPath(QString &pathName);

    int currentPart();
    void setCurrentPart(int index);

    DocumentType documentType() const;

    QSize documentSize() const;
    QImage paintTile(QSize canvasSize, QRect tileSize, qreal zoom = 1.0);

    int partsCount();
 
    QString getPartName(int index) const;
    int swapCurrentPart(int newPartIndex);
 
    void setPart(int index);

    Q_INVOKABLE bool saveAs(QString url, QString format, QString filterOptions);

Q_SIGNALS:
    void pathChanged();
    void currentPartChanged();
    void documentTypeChanged();

private:
    QString m_path;
    DocumentType m_docType;

    bool loadDocument(QString &pathNAme);

    lok::Document *m_document;

    static lok::Office *s_office;
};

#endif // LODOCUMENT_H
