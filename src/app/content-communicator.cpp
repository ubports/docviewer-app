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

#include "content-communicator.h"

#include <QApplication>
#include <QStandardPaths>
#include <QMimeDatabase>
#include <QDebug>
#include <QFileInfo>

#include <com/ubuntu/content/hub.h>
#include <com/ubuntu/content/item.h>
#include <com/ubuntu/content/transfer.h>


using namespace com::ubuntu::content;

/*!
 * \brief ContentCommunicator::ContentCommunicator
 * \param parent
 */
ContentCommunicator::ContentCommunicator(QObject *parent)
    : ImportExportHandler(parent),
      m_transfer(nullptr)
{
}

/*!
 * \brief ContentCommunicator::registerWithHub Register the handlers provided
 * by ContentCommunicator with the content hub
 */
void ContentCommunicator::registerWithHub()
{
    Hub *hub = Hub::Client::instance();
    hub->register_import_export_handler(this);
}

/*!
 * \brief \reimp
 */
void ContentCommunicator::handle_import(content::Transfer *transfer)
{
    // FIXME: If a file is imported from $HOME/Documents, a new copy of the file is created.
    //   Could be use md5? http://doc.qt.io/qt-5/qml-qtqml-qt.html#md5-method
    QVariantList importedDocuments;
    QVector<Item> transferedItems = transfer->collect();
    foreach (const Item &hubItem, transferedItems) {
        QFileInfo fi(hubItem.url().toLocalFile());

        QString dir;
        QString destination;
        bool rejected = false;

        QMimeDatabase mdb;
        QMimeType mt = mdb.mimeTypeForFile(hubItem.url().toLocalFile());

        // Check if the item is supported by Ubuntu Document Viewer
        if (isSupportedMimetype(mt.name())) {
            /* We don't support formats that use a double extension
               (e.g. tar.gz), so we can safely use completeBaseName() and
               suffix() functions, in order to properly detect the name of
               the document even when there's a dot in the middle of the name.*/
            QString suffix = fi.suffix();
            QString filenameWithoutSuffix = fi.completeBaseName();

            if(suffix.isEmpty()) {
                // If the filename doesn't have an extension add one from the
                // detected mimetype
                if(!mt.preferredSuffix().isEmpty()) {
                    suffix = mt.preferredSuffix();
                }
            }

            dir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + QDir::separator();
            destination = QString("%1.%2").arg(dir + filenameWithoutSuffix, suffix);

            // If we already have a file of this name reformat to "filename (copy x).png"
            // (where x is a number, incremented until we find an available filename)
            if(QFile::exists(destination)) {
                /*
                 TRANSLATORS: This string is used for renaming a copied file,
                 when a file with the same name already exists in user's
                 Documents folder.

                 e.g. "Manual_Aquaris_E4.5_ubuntu_EN.pdf" will become
                      "Manual_Aquaris_E4.5_ubuntu_EN (copy 2).pdf"

                      where "2" is given by the argument "%1"
                */
                QString reformattedSuffix = QString(_("copy %1"));

                QRegExp rx(" \\(" + reformattedSuffix.arg(QString("\\d+")) + "\\)");
                int reformattedSuffixPos = filenameWithoutSuffix.lastIndexOf(rx);

                // Check if the file has already a "copy" suffix
                if(reformattedSuffixPos != -1) {
                    // Remove the "copy" suffix. We will re-put it later.
                    filenameWithoutSuffix.truncate(reformattedSuffixPos);
                }

                int append = 1;
                do {
                    destination = QString("%1 (%2).%3").arg(dir + filenameWithoutSuffix,
                                                            reformattedSuffix.arg(QString::number(append)),
                                                            suffix);
                    append++;
                } while(QFile::exists(destination));
            }

            QFile::copy(hubItem.url().toLocalFile(), destination);
        } else {
            rejected = true;
        }

        // Append an entry for the imported document in the list that will be
        // emitted with the 'documentImported' signal.
        QVariantMap entry;
        if (rejected) {
            entry["fileName"] = fi.fileName();
        } else {
            entry["fileName"] = destination;
        }
        entry["rejected"] = rejected;

        importedDocuments.append(entry);
    }

    // Allow content-hub to clean up temporary files in .cache/ once we've
    // moved them
    transfer->finalize();

    emit documentImported(importedDocuments);
}

/*!
 * \brief \reimp
 */
void ContentCommunicator::handle_export(content::Transfer *transfer)
{
    if (m_transfer != nullptr) {
        qWarning() << "docviewer-app does only one content export at a time";
        transfer->abort();
        return;
    }

    m_transfer = transfer;
    emit documentRequested();
    emit selectionTypeChanged();
    emit singleContentPickModeChanged();
}

/*!
 * \brief \reimp
 */
void ContentCommunicator::handle_share(content::Transfer *)
{
    qDebug() << Q_FUNC_INFO << "docviewer does not share content";
}

/*!
 * \brief ContentCommunicator::cancelTransfer aborts the current transfer
 */
void ContentCommunicator::cancelTransfer()
{
    if (!m_transfer) {
        qWarning() << "No ongoing transfer to cancel";
        return;
    }

    m_transfer->abort();
    m_transfer = nullptr;
}

/*!
 * \brief ContentCommunicator::returnSocuments returns the given documents
 * via content hub to the requester
 * \param urls
 */
void ContentCommunicator::returnDocuments(const QVector<QUrl> &urls)
{
    if (!m_transfer) {
        qWarning() << "No ongoing transfer to return a document";
        return;
    }

    QVector<Item> items;
    items.reserve(urls.size());
    foreach (const QUrl &url, urls) {
        items.append(Item(url));
    }

    m_transfer->charge(items);
    m_transfer = nullptr;
}

/*!
 * \brief ContentCommunicator::selectionType return if the transfer requests
 * one single item only, or multiple
 * \return
 */
ContentCommunicator::SelectionType ContentCommunicator::selectionType() const
{
    if (!m_transfer)
        return SingleSelect;

    return static_cast<SelectionType>(m_transfer->selectionType());
}

/*!
 * \brief ContentCommunicator::singleContentPickMode
 * \return
 */
bool ContentCommunicator::singleContentPickMode() const
{
    if (!m_transfer)
        return true;

    // FIXME: Shouldn't be Transfer::SelectionType::SingleSelect?
    return m_transfer->selectionType() == Transfer::SelectionType::single;
}

/*!
 * \brief ContentCommunicator::isSupportedMimetype returns true if the given
 * mimetype is supported by Ubuntu Document Viewer
 * \param mimetype
 */
bool ContentCommunicator::isSupportedMimetype(QString mimetype)
{
    // TODO: We should use a common shared code for DocumentViewer.DocumentsModel
    // QML component and ContentHub. That will happen when we'll switch to
    // QML ContentHub APIs.
    return (mimetype.startsWith("text/")
            || mimetype == "application/pdf"
            || mimetype.startsWith("application/vnd.oasis.opendocument")
            || mimetype == "application/msword")
            || mimetype == "application/vnd.openxmlformats-officedocument.wordprocessingml.document"
            || mimetype == "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"
            || mimetype == "application/vnd.openxmlformats-officedocument.presentationml.presentation"
            || mimetype == "application/msword"
            || mimetype == "application/vnd.ms-excel"
            || mimetype == "application/vnd.ms-powerpoint";
}
