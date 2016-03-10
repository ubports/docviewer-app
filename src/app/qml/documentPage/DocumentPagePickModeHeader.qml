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
import Ubuntu.Content 1.1

PageHeader {
    id: pickModeHeader

    property var view: parent.view
    property Page parentPage: parent

    leadingActionBar {
        anchors.leftMargin: 0
        delegate: textualButton

        actions: Action {
            text: i18n.tr("Cancel")
            objectName: "cancelButton"
            iconName: "close"
            onTriggered: {
                if (!contentHubProxy.activeExportTransfer)
                    return

                contentHubProxy.activeExportTransfer.items = []
                contentHubProxy.activeExportTransfer.state = ContentTransfer.Aborted

                mainView.switchToBrowseMode()
            }
        }
    }

    trailingActionBar {
        anchors.rightMargin: 0
        delegate: textualButton

        actions: Action {
            text: i18n.tr("Pick")
            objectName: "pickButton"
            enabled: view.ViewItems.selectedIndices.length > 0
            iconName: "ok"
            onTriggered: {
                if (!enabled || !contentHubProxy.activeExportTransfer)
                    return;

                var contentList = []
                var indices = view.ViewItems.selectedIndices

                console.log("[content-hub] Following files will be exported:")

                for (var i=0; i < indices.length; i++) {
                    var filePath = "file://" + folderModel.get(i).path
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
        id: textualButton
        AbstractButton {
            id: button
            action: modelData
            width: label.width + units.gu(4)
            height: parent.height
            Rectangle {
                color: UbuntuColors.slate
                opacity: 0.1
                anchors.fill: parent
                visible: button.pressed
            }
            Label {
                anchors.centerIn: parent
                id: label
                text: action.text
                font.weight: text === i18n.tr("Pick") ? Font.Normal : Font.Light
                color: {
                    if (button.enabled)
                        return text === i18n.tr("Pick") ? theme.palette.selected.backgroundText : theme.palette.normal.backgroundText

                    return theme.palette.disabled.backgroundText
                }
            }
        }
    }

    Component {
        id: contentItem
        ContentItem {}
    }
}
