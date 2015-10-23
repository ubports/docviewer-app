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

import QtQuick 2.4
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.3
import DocumentViewer 1.0
import QtQuick.Window 2.0
import Qt.labs.settings 1.0

import "common"
import "common/loadComponent.js" as LoadComponent

MainView {
    id: mainView
    objectName: "mainView"

    property bool pickMode: commandLineProxy.pickMode
    property bool fullscreen: commandLineProxy.fullscreen
    readonly property bool isLandscape: Screen.orientation == Qt.LandscapeOrientation ||
                                        Screen.orientation == Qt.InvertedLandscapeOrientation

    applicationName: "com.ubuntu.docviewer"
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

    function toggleFullScreen() {
        mainView.fullscreen = !mainView.fullscreen
    }

    function setHeaderVisibility(visible, toggleFullscreen) {
        toggleFullscreen = typeof toggleFullscreen !== 'undefined' ? toggleFullscreen : true
        header.visible = visible;

        // If device orientation is landscape and screen width is limited,
        // force hiding Unity 8 indicators panel.
        if (!DocumentViewer.desktopMode && mainView.isLandscape &&
                mainView.width < units.gu(51)) {
            mainView.fullscreen = true;
            return;
        }

        if (!DocumentViewer.desktopMode && toggleFullscreen)
            mainView.fullscreen = !visible;
    }

    function toggleHeaderVisibility() {
        setHeaderVisibility(!header.visible);
    }

    function switchToBrowseMode() {
        mainView.pickMode = false
    }

    function switchToPickMode() {
        mainView.pickMode = true
    }

    // On screen rotation, force updating of header/U8 indicators panel visibility
    onIsLandscapeChanged: setHeaderVisibility(true);

    onFullscreenChanged: {
        if (mainView.fullscreen)
            window.visibility = Window.FullScreen
        else
            window.visibility = Window.Windowed
    }

    Component.onCompleted: {
        pageStack.push(Qt.resolvedUrl("documentPage/DocumentPage.qml"));

        // Open the document, if one has been specified.
        openDocument(commandLineProxy.documentFile);
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
            customDir: commandLineProxy.documentsDir
        }

        // TODO: Expose an enum from DocumentViewer module.
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

    // CommandLine parser
    CommandLineProxy {
        id: commandLineProxy
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
        onOpened: openDocument(uris[0])
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
