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
import com.ubuntu.fileqmlplugin 1.0
import DocumentViewer 1.0

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

            file.path = path;
        }
    }

    function runUnknownTypeDialog() {
        PopupUtils.open(Qt.resolvedUrl("common/UnknownTypeDialog.qml"), mainView, { parent: mainView });
    }

    Component.onCompleted: {
        pageStack.push(Qt.resolvedUrl("documentPage/DocumentPage.qml"));

        // Open the document, if one has been specified.
        openDocument(DOC_VIEWER.documentFile);
    }

    File {
        id: file
        objectName: "file"

        onMimetypeChanged: LoadComponent.load(mimetype)
        onErrorChanged: { if (error == -1); PopupUtils.open(Qt.resolvedUrl("common/ErrorDialog.qml"), mainView, { parent: mainView }) }
    }

    DocumentsModel { id: folderModel }
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
}
