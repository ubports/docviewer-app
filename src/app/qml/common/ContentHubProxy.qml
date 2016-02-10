/*
 * Copyright (C) 2012-2014 Canonical, Ltd.
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
 */

import QtQuick 2.4
import Ubuntu.Content 1.1
import DocumentViewer 1.0

// TODO: Show a dialog asking for the destination (internal storage or SD card)

Item {
    id: contentHubProxy

    property var activeImportTransfer
    property var activeExportTransfer

    // This property is used in ../documentPage/Document(Grid|List)View.qml
    // so that we avoid to import Ubuntu.Content module outside this proxy.
    property bool multipleSelectionType: !activeExportTransfer || activeExportTransfer.selectionType == ContentTransfer.Multiple

    property alias rejectedDocuments: rejectedDocsModel
    property alias importedDocuments: importedDocsModel

    ListModel { id: rejectedDocsModel }
    ListModel { id: importedDocsModel }

    ContentTransferHint {
        activeTransfer: contentHubProxy.activeImportTransfer
    }

    Connections {
        target: ContentHub

        onImportRequested: {
             activeImportTransfer = transfer

            if (activeImportTransfer.state === ContentTransfer.Charged) {
                mainView.switchToBrowseMode()

                internal.clearModels()

                for (var i=0; i<activeImportTransfer.items.length; i++) {
                    var sourcePath = internal.getPathFromUrl(activeImportTransfer.items[i].url)

                    if (DocumentViewer.isFileSupported(sourcePath)) {
                        var documentsLocation = DocumentViewer.getXdgDocumentsLocation()

                        // Check if we have already imported the same document in the past.
                        var earlierImportedFile = DocumentViewer.checkIfFileAlreadyImported(sourcePath, [documentsLocation])
                        if (earlierImportedFile.length > 0) {
                            // Document has been already imported in the past.
                            // Append the path of the earlier copy of the
                            // document in our model, so we can open it instead.
                            importedDocsModel.append({ path: earlierImportedFile })
                        } else {
                            // No document has been found, so we can safely copy it.
                            var destPath = DocumentViewer.buildDestinationPath(documentsLocation, sourcePath);

                            internal.importDocument(sourcePath, destPath)
                        }
                    } else {
                        // Document is not supported, append its entry into the
                        // rejected documents model, so that we can inform the
                        // user of what happened.
                        rejectedDocsModel.append({ path: sourcePath })
                    }
                }

                internal.finalizeImport()

                internal.handleNotifications()
            }
        }

        onExportRequested: {
            activeExportTransfer = transfer
            mainView.switchToPickMode()
        }
    }

    QtObject {
        id: internal

        function __openDocument() {
            if (contentHubProxy.importedDocuments.count > 1) {
                // If it has been imported more than a document, show
                // a file picker when user taps the "open" action.
                PopupUtils.open(
                            Qt.resolvedUrl("common/PickImportedDialog.qml"),
                            mainView,
                            {
                                parent: mainView,
                                model: contentHubProxy.importedDocuments
                            })
            } else {
                // It has been imported just a document, open it when
                // user taps the action button.
                mainView.openDocument(contentHubProxy.importedDocuments.get(0).path)
            }
        }

        function clearModels() {
            rejectedDocsModel.clear()
            importedDocsModel.clear()
        }

        function getPathFromUrl(url) {
            return url.toString().replace("file://", "")
        }

        function importDocument(sourcePath, destPath) {
            DocumentViewer.copy(sourcePath, destPath)
            importedDocsModel.append({ path: destPath })
        }

        function finalizeImport() {
            activeImportTransfer.finalize()
        }

        function handleNotifications() {
            // Check if there's any rejected document in the last transfer.
            // If so, show an error dialog.
            if (contentHubProxy.rejectedDocuments.count > 0) {
                var rejectedDialog = PopupUtils.open(
                            Qt.resolvedUrl("common/RejectedImportDialog.qml"),
                            mainView,
                            {
                                parent: mainView,
                                model: contentHubProxy.rejectedDocuments
                            })
                rejectedDialog.closed.connect(openDocument)
            } else {
                // Open the document, or show a pick dialog if more than one have been imported.
                __openDocument()
            }
        }
    }
}
