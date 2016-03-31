/*
 * Copyright (C) 2014-2016 Canonical, Ltd.
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

import "../common"

PageHeader {
    id: selectionHeader

    property var view: parent.view
    property Page parentPage: parent

    property bool selectedAll: view.ViewItems.selectedIndices.length == view.count

    leadingActionBar.actions: Action {
        iconName: "close"
        text: i18n.tr("Close")
        onTriggered: {
            view.cancelSelection()
        }
    }

    trailingActionBar {
        anchors.rightMargin: 0
        delegate: TextualButtonStyle {}

        actions: [
            Action {
                iconName: selectedAll ? "select-none" :  "select"
                text: selectedAll ? i18n.tr("Select None") : i18n.tr("Select All")

                onTriggered: {
                    if (selectedAll)
                        view.clearSelection()
                    else
                        view.selectAll()
                }
            },

            Action {
                iconName: "delete"
                text: i18n.tr("Delete")
                enabled: view.ViewItems.selectedIndices.length !== 0

                onTriggered: PopupUtils.open(Qt.resolvedUrl("DeleteFileDialog.qml"), documentPage)
            }
        ]
    }
}
