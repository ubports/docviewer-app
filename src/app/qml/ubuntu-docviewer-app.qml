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
import QtQuick.Window 2.0
import Qt.labs.settings 1.0

import "common"
import "common/loadComponent.js" as LoadComponent

MainView {
    id: mainView
    objectName: "mainView"

    // TODO: Connect with arguments
    property bool pickMode: false
    readonly property bool isLandscape: Screen.orientation == Qt.LandscapeOrientation ||
                                        Screen.orientation == Qt.InvertedLandscapeOrientation

    applicationName: "com.ubuntu.docviewer"
    useDeprecatedToolbar: false   
    automaticOrientation: true

    width: units.gu(150)
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

    function setFullScreen(fullScreen) {
        DOC_VIEWER.fullScreen = fullScreen;
    }

    function toggleFullScreen() {
        DOC_VIEWER.fullScreen = !DOC_VIEWER.fullScreen;
    }

    function setHeaderVisibility(visible, toggleFullscreen) {
        toggleFullscreen = typeof toggleFullscreen !== 'undefined' ? toggleFullscreen : true
        header.visible = visible;

        // If device orientation is landscape and screen width is limited,
        // force hiding Unity 8 indicators panel.
        if (!DOC_VIEWER.desktopMode && mainView.isLandscape &&
                mainView.width < units.gu(51)) {
            setFullScreen(true);
            return;
        }

        if (!DOC_VIEWER.desktopMode && toggleFullscreen)
            setFullScreen(!visible);
    }

    function toggleHeaderVisibility() {
        setHeaderVisibility(!header.visible);
    }

    function setPickMode(pickMode) {
        mainView.pickMode = pickMode
    }

    function switchToBrowseMode() {
        setPickMode(false)
    }

    function switchToPickMode() {
        setPickMode(true)
    }


    // On screen rotation, force updating of header/U8 indicators panel visibility
    onIsLandscapeChanged: setHeaderVisibility(true);

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

        function search(pattern) {
            // Search the given pattern, case insensitive
            filter.pattern = new RegExp(pattern, 'i')
        }

        model: DocumentsModel {
            id: docModel

            // Used for autopilot tests! If customDir is empty, this property is not used.
            customDir: DOC_VIEWER.documentsDir
        }

        sort.property: {
            switch (sortSettings.sortMode) {
            case 0:
                return "date"
            case 1:
                return "name"
            case 2:
                return "size"
            default:
                return "date"
            }
        }
        sort.order: {
            switch (sortSettings.sortMode) {
            case 0:     // sort by date
                return sortSettings.reverseOrder ? Qt.AscendingOrder : Qt.DescendingOrder
            case 1:     // sort by name
                return sortSettings.reverseOrder ? Qt.DescendingOrder : Qt.AscendingOrder
            case 2:     // sort by size
                return sortSettings.reverseOrder ? Qt.DescendingOrder : Qt.AscendingOrder
            default:
                return sortSettings.reverseOrder ? Qt.AscendingOrder : Qt.DescendingOrder
            }
        }
        sortCaseSensitivity: Qt.CaseInsensitive

        filter.property: "name"
    }

    PageStack { id: pageStack }

    Settings {
        id: sortSettings

        property int sortMode: 0    // 0 = by date, 1 = by name, 2 = by size
        property bool reverseOrder: false
    }

    // Content Hub support
    property alias contentHubProxy: contentHubLoader.item
    Loader {
        id: contentHubLoader

        asynchronous: true
        source: Qt.resolvedUrl("common/ContentHubProxy.qml")
    }

    // Uri Handler support
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
    }

    onPickModeChanged: {
        if (mainView.pickMode) {
            // If a document is loaded, pop() its page.
            while (pageStack.depth > 1) {
                pageStack.pop()
            }
        }
    }

    property bool nightModeEnabled: false
    layer.effect: NightModeShader {}
    layer.enabled: nightModeEnabled && (pageStack.depth > 1)
}
