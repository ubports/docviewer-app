/*
  Copyright (C) 2015 Stefano Verzegnassi

    This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License 3 as published by
  the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
  along with this program. If not, see http://www.gnu.org/licenses/.
*/

import QtQuick 2.4
import Ubuntu.Components 1.3

ListView {
    id: documentListView
    anchors { fill: parent; margins: units.gu(0.5) }
    clip: true

    function isMultipleSelection() {
        // No active transfer, then we're in selection mode.
        if (!contentHubProxy.activeTransfer)
            return true

        // We have an active transfer, get the value from the content hub proxy
        return contentHubProxy.multipleSelectionType
    }

    function clearSelection() {
        ViewItems.selectedIndices = []
    }

    function selectAll() {
        clearSelection()

        for (var i=0; i < model.count; i++) {
            ViewItems.selectedIndices.push(i)
        }
    }

    function cancelSelection() {
        clearSelection()
        ViewItems.selectMode = false
    }

    function startSelection() {
        ViewItems.selectMode = true
    }

    model: folderModel
    delegate: DocumentListDelegate {
        onClicked: {
            if (selectMode) {
                if (!isMultipleSelection())
                    clearSelection()

                selected = !selected
                return;
            }

            mainView.openDocument(model.path)
        }

        onPressAndHold: {
            ListView.view.ViewItems.selectMode = !ListView.view.ViewItems.selectMode
            selected = true
        }
    }

    section {
        delegate: SectionHeader { }
        property: {
            switch (sortSettings.sortMode) {
            case 0:     // sort by date
                return "dateDiff"
            case 1:     // sort by name
                return "name"
            default:    // sort by size -> do not show section header
                return ""
            }
        }
        criteria: {
            if (sortSettings.sortMode === 1)    // sort by name
                return ViewSection.FirstCharacter

            return ViewSection.FullString
        }
    }

    Scrollbar {
        flickableItem: documentListView
        parent: documentListView.parent
    }

    Connections {
        target: sortSettings
        onSortModeChanged: documentListView.positionViewAtBeginning()
    }

    Component.onCompleted: if (mainView.pickMode) documentListView.startSelection()
}
