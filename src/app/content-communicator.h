/*
 * Copyright (C) 2013 Canonical, Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef CONTENTCOMMUNICATOR_H
#define CONTENTCOMMUNICATOR_H

#include <com/ubuntu/content/import_export_handler.h>
#include <com/ubuntu/content/transfer.h>

#include <QUrl>
#include <QVector>

using namespace com::ubuntu;

/*!
 * Class to handle the communication with the content manager
 */
class ContentCommunicator : public content::ImportExportHandler
{
    Q_OBJECT
    Q_PROPERTY(bool singleContentPickMode READ singleContentPickMode NOTIFY singleContentPickModeChanged)
    Q_PROPERTY(SelectionType selectionType READ selectionType NOTIFY selectionTypeChanged)
    Q_ENUMS(SelectionType)

public:
    enum SelectionType {
        SingleSelect = content::Transfer::single,
        MultiSelect = content::Transfer::multiple
    };

    ContentCommunicator(QObject *parent = nullptr);

    virtual void handle_import(content::Transfer*);
    virtual void handle_export(content::Transfer *transfer);
    virtual void handle_share(content::Transfer*);

    void cancelTransfer();
    void returnDocuments(const QVector<QUrl> &urls);

    SelectionType selectionType() const;
    bool singleContentPickMode() const;

    void registerWithHub();

signals:
    void documentRequested();
    void documentImported(QList<QVariantMap> documents);
    void selectionTypeChanged();
    void singleContentPickModeChanged();

private:
    content::Transfer *m_transfer;

    bool isSupportedMimetype(QString mimetype);
};

#endif // CONTENTCOMMUNICATOR_H
