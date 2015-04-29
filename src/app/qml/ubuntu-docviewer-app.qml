/*
 * Copyright (C) 2013-2015 Canonical, Ltd.
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

import QtQuick 2.3
import Ubuntu.Components 1.1
import Ubuntu.Components.Popups 1.0
import DocumentViewer 1.0

import "common"
import "common/loadComponent.js" as LoadComponent

MainView {
    id: mainView
    objectName: "mainView"

    applicationName: "com.ubuntu.docviewer"
    useDeprecatedToolbar: false

    property bool pickMode: DOC_VIEWER.pickModeEnabled

    width: units.gu(50)
    height: units.gu(75)

    function openDocument(path)  {
        if (path !== "") {
            console.log("Path of the document:", path)

            // If a document is already shown, pop() its page.
            while (pageStack.depth > 1)
                pageStack.pop();

            path = path.replace("file://", "")
                       .replace("document://", "");

            if (file.path === path) {
                // File has been already initialized, so just open the viewer
                LoadComponent.load(file.mimetype.name);

                return
            }

            file.path = path;
        }
    }

    function runUnknownTypeDialog() {
        PopupUtils.open(Qt.resolvedUrl("common/UnknownTypeDialog.qml"),
                        mainView, { parent: mainView });
    }

    function showNotification(args) {
        var component = Qt.createComponent("common/Toast.qml")
        var toast = component.createObject(mainView, args);

        return toast;
    }

    function showNotificationWithAction(args) {
        var component = Qt.createComponent("common/ToastWithAction.qml")
        var toast = component.createObject(mainView, args);

        return toast;
    }

    function setFullScreen(fullScreen) {
        DOC_VIEWER.fullScreen = fullScreen;
    }

    function toggleFullScreen() {
        DOC_VIEWER.fullScreen = !APP.fullScreen;
    }

    function setHeaderVisibility(visible, toggleFullscreen) {
        toggleFullscreen = typeof toggleFullscreen !== 'undefined' ? toggleFullscreen : true
        header.visible = visible;
        if (!DOC_VIEWER.desktopMode && toggleFullscreen)
            setFullScreen(!visible);
    }

    function toggleHeaderVisibility() {
        setHeaderVisibility(!header.visible);
    }

    Component.onCompleted: {
        pageStack.push(Qt.resolvedUrl("documentPage/DocumentPage.qml"));

        // Open the document, if one has been specified.
        openDocument(DOC_VIEWER.documentFile);
    }

    File {
        id: file
        objectName: "file"

        onMimetypeChanged: LoadComponent.load(mimetype.name)
        onErrorChanged: {
            if (error == -1)
                PopupUtils.open(Qt.resolvedUrl("common/FileNotFoundDialog.qml"),
                                mainView, { parent: mainView });
        }
    }

    SortFilterModel {
        id: folderModel
        model: DocumentsModel {
            id: docModel

            // Used for autopilot tests! If customDir is empty, this property is not used.
            customDir: DOC_VIEWER.documentsDir
        }

        sort.property: "date"
        sort.order: Qt.DescendingOrder
        sortCaseSensitivity: Qt.CaseSensitive
    }

    PageStack { id: pageStack }

    Connections {
        target: UriHandler
        onOpened: {
            for (var i = 0; i < uris.length; ++i) {
                DOC_VIEWER.parseUri(uris[i])
            }
        }
    }

    Connections {
        target: DOC_VIEWER

        onDocumentFileChanged: {
            openDocument(DOC_VIEWER.documentFile);
        }

        onPickModeEnabledChanged: {
            mainView.pickMode = DOC_VIEWER.pickModeEnabled

            if (mainView.pickMode) {
                // If a document is loaded, pop() its page.
                while (pageStack.depth > 1) {
                    pageStack.pop()
                }
            }
        }
    }

    Connections {
        target: PICKER_HUB

        onDocumentImported: {
            // Create two arrays: one for rejected documents, and the other
            // for imported documents.
            var importedDocuments = [];
            var rejectedDocuments = [];
            var entry;

            // Fill the arrays.
            for (var i=0; i<documents.length; i++) {
                entry = documents[i];

                if (entry.rejected) {
                    rejectedDocuments.push(entry.fileName);
                    break;
                }

                importedDocuments.push(entry.fileName);
            }

            // Prepare import notification
            var showImportNotification = function() {
                if (importedDocuments.length > 0) {
                    var importDialog = showNotificationWithAction({
                        "text": i18n.tr("Document successfully imported!",
                                        "Documents successfully imported!",
                                        importedDocuments.length),
                        "action.text": i18n.tr("Open")
                    })

                    if (importedDocuments.length > 1) {
                        // If it has been imported more than a document, show
                        // a file picker when user taps the "open" action.
                        importDialog.action.triggered.connect(function() {
                            PopupUtils.open(
                                Qt.resolvedUrl("common/PickImportedDialog.qml"),
                                mainView,
                                {
                                    parent: mainView,
                                    model: importedDocuments
                                }
                            );
                        });
                    } else {
                        // It has been imported just a document, open it when
                        // user taps the action button.
                        importDialog.action.triggered.connect(function() {
                            openDocument(importedDocuments[0]);
                        });
                    }
                }
            }

            // Check if there's any rejected document in the last transfer.
            // If so, show an error dialog.
            if (rejectedDocuments.length > 0) {
                var rejectedDialog = PopupUtils.open(
                    Qt.resolvedUrl("common/RejectedImportDialog.qml"),
                    mainView,
                    {
                        parent: mainView,
                        model: rejectedDocuments
                    }
                );

                // Show import notification after the dialog has been closed.
                rejectedDialog.closed.connect(showImportNotification)
            } else {
                // No dialog has been shown. Show the notification.
                showImportNotification.call();
            }
        }
    }

    property bool nightModeEnabled: false
    layer.effect: NightModeShader {}
    layer.enabled: nightModeEnabled && (pageStack.depth > 1)
}
