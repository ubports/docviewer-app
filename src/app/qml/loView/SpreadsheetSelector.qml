/*
 * Copyright (C) 2016 Stefano Verzegnassi
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
import DocumentViewer.LibreOffice 1.0

import "../common"

Item {
    id: spreadsheetSelector
    anchors { left: parent.left; right: parent.right }
    height: visible ? units.gu(6) : 0

    property Viewer view

    Rectangle {
        anchors.fill: parent
        color: theme.palette.normal.background
    }

    ListView {
        id: listView
        anchors.fill: parent
        boundsBehavior: Flickable.StopAtBounds
        orientation: ListView.Horizontal

        model: spreadsheetSelector.view.partsModel

        delegate: AbstractButton {
            id: del
            property bool selected: spreadsheetSelector.view.currentPart == model.index

            width: nameLabel.paintedWidth + units.gu(4)
            height: parent.height

            onClicked: spreadsheetSelector.view.currentPart = model.index

            Rectangle {
                anchors.fill: parent
                visible: del.selected
                color: theme.palette.selected.background
            }

            Label {
                id: nameLabel
                anchors.centerIn: parent

                // TRANSLATORS: Please don't add any space between "Sheet" and "%1".
                // This is the default name for a sheet in LibreOffice.
                text: model.name || i18n.tr("Sheet%1").arg(model.index + 1)
                color: del.selected ? theme.palette.selected.backgroundText
                                    : theme.palette.normal.backgroundTertiaryText
            }

            Rectangle {
                anchors {
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom
                }
                height: units.dp(2)
                color: theme.palette.selected.backgroundText
                visible: del.selected
            }
        }
    }

    HorizontalDivider {
        anchors { left: parent.left; right: parent.right }
    }
}
