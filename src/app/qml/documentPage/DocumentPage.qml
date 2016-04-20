/*
 * Copyright (C) 2015, 2016 Canonical, Ltd.
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
import Qt.labs.settings 1.0

Page {
    id: documentPage

    property bool searchMode: false
    property alias view: view

    header: defaultHeader

    onActiveChanged: {
        // When the page become visible, check if any new volume has been
        // mounted in the file system.
        if (active)
            docModel.checkDefaultDirectories();
    }

    ScrollView {
        id: scrollView
        anchors.fill: parent
        anchors.topMargin: documentPage.header.height

        DocumentListView {
            id: view
            anchors.fill: parent
        }
    }

    Loader {
        id: emptyStateLoader
        anchors.fill: parent
        active: folderModel.count == 0
        source: documentPage.searchMode
                ? Qt.resolvedUrl("SearchEmptyState.qml")
                : Qt.resolvedUrl("DocumentEmptyState.qml")
    }


    /*** Headers ***/

    DocumentPageDefaultHeader {
        id: defaultHeader
        visible: !mainView.pickMode && !view.ViewItems.selectMode && !documentPage.searchMode
        enabled: visible
    }

    DocumentPagePickModeHeader {
        id: pickModeHeader
        visible: mainView.pickMode
        enabled: visible
    }

    DocumentPageSearchHeader {
        id: searchHeader
        visible: !mainView.pickMode && !view.ViewItems.selectMode && documentPage.searchMode
        enabled: visible
    }

    DocumentPageSelectionModeHeader {
        id: selectionHeader
        visible: !mainView.pickMode && view.ViewItems.selectMode
        enabled: visible
    }

    Connections {
        target: mainView

        onPickModeChanged: {
            if (mainView.pickMode) {
                view.startSelection()
            } else {
                view.cancelSelection()
            }

            // Reset any previous search
            documentPage.searchMode = false
            folderModel.search("")  // Empty search, reset filter.
        }
    }
}
