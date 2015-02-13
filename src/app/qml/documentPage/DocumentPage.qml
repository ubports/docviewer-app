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

import QtQuick 2.0
import Ubuntu.Components 1.1
import Ubuntu.Components.Popups 1.0
import Qt.labs.settings 1.0

Page {
    id: documentPage

    title: i18n.tr("Document Viewer")
    flickable: null

    property bool useGridView: false

    Settings {
        property alias useGridView: documentPage.useGridView
    }

    property alias view: viewLoader
    Loader {
        id: viewLoader
        anchors.fill: parent

        source: (folderModel.count === 0) ? Qt.resolvedUrl("./DocumentEmptyState.qml")
                                     : useGridView ? Qt.resolvedUrl("./DocumentGridView.qml")
                                                   : Qt.resolvedUrl("./DocumentListView.qml")
    }

    // *** HEADER ***
    states: [
        DocumentPageDefaultHeader {
            name: "default"
            targetPage: documentPage
            when: !mainView.pickMode && !viewLoader.item.isInSelectionMode
        },

        DocumentPagePickModeHeader {
            name: "pickMode"
            targetPage: documentPage
            when: mainView.pickMode
        },

        DocumentPageSelectionModeHeader {
            name: "selection"
            targetPage: documentPage
            when: !mainView.pickMode && viewLoader.item.isInSelectionMode
        }
    ]

    Connections {
        target: DOC_VIEWER

        onPickModeEnabledChanged: {
            if (DOC_VIEWER.pickModeEnabled) {
                viewLoader.item.startSelection()
            } else {
                viewLoader.item.cancelSelection()
            }
        }
    }
}
