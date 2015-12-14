/*
 * Copyright (C) 2015 Stefano Verzegnassi
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
import QtQuick.Layouts 1.1
import DocumentViewer.LibreOffice 1.0 as LibreOffice

ListView {
    id: view
    objectName: "view"
    clip: true

    property bool expanded: true

    orientation: ListView.Vertical

    // used in vertical mode
    property bool isWide: width > units.gu(24)

    currentIndex: view.model ? loView.currentPart : -1
    highlightMoveDuration: UbuntuAnimation.SnapDuration

    delegate: (orientation == ListView.Vertical) ? verticalDelegate : horizontalDelegate

    Component {
        id: verticalDelegate

        ListItem {
            id: delegate
            width: parent.width
            height: units.gu(16)

            color: (loView.currentPart === model.index) ? theme.palette.selected.background
                                                        : "transparent"

            onClicked: internal.delegate_onClicked(model.index)

            /*** UITK 1.3 specs: Three slot layout (B-A-C)
              ________________________________________
             |   |                                |   |
             | B |               A                | C |
             |___|________________________________|___|

            *********************************************/

            ListItemLayout {
                id: listItemLayout
                anchors.fill: parent

                /* UITK 1.3 specs: Slot B */
                Image {
                    SlotsLayout.position: SlotsLayout.Leading
                    height: parent.height; width: height
                    fillMode: Image.PreserveAspectFit
                    // Do not store a cache of the thumbnail, so that we don't show
                    // thumbnails of a previously loaded document.
                    cache: true // TODO PLAY WITH IT
                    source: model.thumbnail
                }

                /* UITK 1.3 specs: Slot A */
                title {
                    wrapMode: Text.WordWrap
                    text: model.name
                    visible: view.isWide
                    color: (loView.currentPart === model.index) ? UbuntuColors.orange
                                                                : theme.palette.selected.backgroundText
                }

                /* UITK 1.3 specs: Slot C */
                Label {
                    SlotsLayout.position: SlotsLayout.Trailing

                    text: model.index + 1
                    color: (loView.currentPart === model.index) ? UbuntuColors.orange
                                                                : theme.palette.selected.backgroundText
                }
            }
        }
    }

    Component {
        id: horizontalDelegate

        ListItem {
            id: delegate
            height: parent.height; width: height

            color: (loView.currentPart === model.index) ? theme.palette.selected.background
                                                        : "transparent"

            onClicked: internal.delegate_onClicked(model.index)

            ColumnLayout {
                anchors {
                    fill: parent
                    topMargin: units.gu(1)
                    leftMargin: units.gu(2)
                    rightMargin: units.gu(2)
                }
                spacing: units.gu(0.5)

                Image {
                    Layout.fillWidth: true
                    Layout.preferredHeight: width
                    fillMode: Image.PreserveAspectFit
                    // Do not store a cache of the thumbnail, so that we don't show
                    // thumbnails of a previously loaded document.
                    cache: true // TODO PLAY WITH IT
                    source: model.thumbnail
                }

                Label {
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                    text: model.index + 1
                    color: (loView.currentPart === model.index) ? UbuntuColors.orange
                                                                : theme.palette.selected.backgroundText
                }
            }
        }
    }

    QtObject {
        id: internal

        function delegate_onClicked(index) {
            loView.currentPart = index

            // Check if the view has been included in a nested page (e.g.
            // bottomEdge). If so, close that page and return to the
            // main viewer.
            if (view.hasOwnProperty("belongsToNestedPage"))
                pageStack.pop();
        }
    }


    Scrollbar { flickableItem: view; parent: view.parent }
}
