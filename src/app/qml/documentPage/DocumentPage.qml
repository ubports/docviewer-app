/*
 * Copyright (C) 2015 Canonical, Ltd.
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
    title: i18n.tr("Documents")
    flickable: null

    property bool useGridView: false
    property bool searchMode: false
    property alias view: viewLoader

    onActiveChanged: {
        // When the page become visible, check if any new volume has been
        // mounted in the file system.
        if (active)
            docModel.checkDefaultDirectories();
    }

    Settings {
        property alias useGridView: documentPage.useGridView
    }

    Loader {
        id: viewLoader

        width: Math.min(units.gu(80), parent.width)
        anchors {
            top: parent.top
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }

        source: {
            if (folderModel.count === 0) {
                return documentPage.state == "search"
                        ? Qt.resolvedUrl("SearchEmptyState.qml")
                        : Qt.resolvedUrl("BrowserEmptyState.qml")
            }

            return Qt.resolvedUrl("DocumentListView.qml")
        }
    }

    // *** HEADER ***
    states: [
        DocumentPageDefaultHeader {
            name: "default"
            targetPage: documentPage
            when: !mainView.pickMode && !viewLoader.item.ViewItems.selectMode && !documentPage.searchMode
        },

        DocumentPagePickModeHeader {
            name: "pickMode"
            targetPage: documentPage
            when: mainView.pickMode
        },

        DocumentPageSelectionModeHeader {
            name: "selection"
            targetPage: documentPage
            when: !mainView.pickMode && viewLoader.item.ViewItems.selectMode
        },

        DocumentPageSearchHeader {
            name: "search"
            targetPage: documentPage
            when: !mainView.pickMode && !viewLoader.item.ViewItems.selectMode && documentPage.searchMode
        }
    ]

    Connections {
        target: mainView

        onPickModeChanged: {
            if (mainView.pickMode) {
                viewLoader.item.startSelection()
            } else {
                viewLoader.item.cancelSelection()
            }

            // Reset any previous search
            documentPage.searchMode = false
            folderModel.search("")  // Empty search, reset filter.
        }
    }
}
