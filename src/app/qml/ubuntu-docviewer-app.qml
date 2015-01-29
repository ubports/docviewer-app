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

import "loadComponent.js" as LoadComponent

MainView {
    id: mainView
    objectName: "docviewer"

    applicationName: "com.ubuntu.docviewer"
    useDeprecatedToolbar: false
    
    width: units.gu(50)
    height: units.gu(75)

    File {
        objectName: "fileObject"
        id: file

        onMimetypeChanged: LoadComponent.load(mimetype)
        onErrorChanged: { if (error == -1); PopupUtils.open(Qt.resolvedUrl("ErrorDialog.qml"), mainView, { parent: mainView }) }
    }

    Component.onCompleted: {
        // Check if a value has been specified for "documentPath" argument.
        // The value for the argument is parsed in main.cpp.

        if (documentPath) {
            // If so, send the path to the File plugin and load the document.
            console.log("Path argument is:", documentPath);
            file.path = documentPath;
        } else {
            // Otherwise, push a welcome screen in the stack.
            pageStack.push(Qt.resolvedUrl("WelcomePage.qml"));
        }
    }

    // Content Importer
    // Used when user asks to open a document from ContentHub.
    Loader {
        id: contentHubLoader

        asynchronous: true
        source: Qt.resolvedUrl("ContentHubProxy.qml")
        onStatusChanged: {
            if (status === Loader.Ready) {
                item.pageStack = pageStack
            }
        }
    }

    function runUnknownTypeDialog() {
        PopupUtils.open(Qt.resolvedUrl("UnknownTypeDialog.qml"), mainView, { parent: mainView });
    }

    PageStack { id: pageStack }
}
