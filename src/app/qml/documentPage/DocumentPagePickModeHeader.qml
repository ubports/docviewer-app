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
import Ubuntu.Content 1.3

import "../common"

PageHeader {
    id: pickModeHeader

    property var view: parent.view
    property Page parentPage: parent

    leadingActionBar {
        anchors.leftMargin: 0
        delegate: TextualButtonStyle {}

        actions: Action {
            text: i18n.tr("Cancel")
            objectName: "cancelButton"
            onTriggered: {
                if (!contentHubProxy.activeExportTransfer) {
                    console.log("[content-hub] No active transfer.")
                } else {
                    contentHubProxy.activeExportTransfer.items = []
                    contentHubProxy.activeExportTransfer.state = ContentTransfer.Aborted
                }

                mainView.switchToBrowseMode()
            }
        }
    }

    trailingActionBar {
        anchors.rightMargin: 0
        delegate: TextualButtonStyle {}

        actions: Action {
            text: i18n.tr("Pick")
            objectName: "pickButton"
            enabled: view.ViewItems.selectedIndices.length > 0
            onTriggered: {
                if (!enabled || !contentHubProxy.activeExportTransfer) {
                    console.log("[content-hub] No active transfer.")
                    return;
                }

                var contentList = []
                var indices = view.ViewItems.selectedIndices

                console.log("[content-hub] Following files will be exported:")

                for (var i=0; i < indices.length; i++) {
                    var filePath = "file://" + folderModel.get(indices[i]).path
                    console.log(filePath)

                    contentList.push(contentItem.createObject(pickModeHeader, { "url": filePath }))
                }

                contentHubProxy.activeExportTransfer.items = contentList
                contentHubProxy.activeExportTransfer.state = ContentTransfer.Charged

                mainView.switchToBrowseMode()
            }
        }
    }

    Component {
        id: contentItem
        ContentItem {}
    }
}
