/*
 * Copyright (C) 2014-2015 Canonical, Ltd.
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
import QtQuick.Layouts 1.1
import Ubuntu.Components.Popups 1.0

import "../upstreamComponents"

PageHeadState {
    id: rootItem

    property Page targetPage

    head: targetPage.head

    backAction: Action {
        iconName: "close"
        text: i18n.tr("Close")
        onTriggered: {
            viewLoader.item.cancelSelection()
        }
    }

    contents: Loader {
        id: selectionStateLoader
        active: documentPage.state === "selection"
        sourceComponent: Item {
            HeaderButton {
                id: selectButton

                anchors {
                    right: deleteButton.left
                    rightMargin: units.gu(1)
                }

                text: {
                    if(viewLoader.item.selectedItems.count === viewLoader.item.count) {
                        return i18n.tr("Select None")
                    } else {
                        return i18n.tr("Select All")
                    }
                }

                iconSource: {
                    if(viewLoader.item.selectedItems.count === viewLoader.item.count) {
                        return Qt.resolvedUrl("../../graphics/select-none.svg")
                    } else {
                        return Qt.resolvedUrl("../../graphics/select.svg")
                    }
                }

                onTriggered: {
                    if(viewLoader.item.selectedItems.count === viewLoader.item.count) {
                        viewLoader.item.clearSelection()
                    } else {
                        viewLoader.item.selectAll()
                    }
                }
            }

            HeaderButton {
                id: deleteButton

                anchors.right: parent.right
                anchors.rightMargin: units.gu(2)

                iconName: "delete"
                text: i18n.tr("Delete")
                enabled: viewLoader.item.selectedItems.count !== 0

                onTriggered: {
                    PopupUtils.open(Qt.resolvedUrl("DeleteFileDialog.qml"), documentPage)
                }
            }
        }

        height: parent ? parent.height : undefined
        anchors.right: parent ? parent.right: undefined
    }

}
