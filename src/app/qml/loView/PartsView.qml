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
    property bool isWide: width > units.gu(24)

    currentIndex: view.model ? loView.document.currentPart : -1
    highlightMoveDuration: UbuntuAnimation.SnapDuration

    delegate: ListItem {
        id: delegate

        width: parent.width
        height: units.gu(16)

        color: (loView.document.currentPart === model.index) ? theme.palette.selected.background
                                                             : "transparent"

        onClicked: {
            loView.document.currentPart = model.index

            // Check if the view has been included in a nested page (e.g.
            // bottomEdge). If so, close that page and return to the
            // main viewer.
            if (view.hasOwnProperty("belongsToNestedPage"))
                pageStack.pop();
        }

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
                color: (loView.document.currentPart === model.index) ? UbuntuColors.orange
                                                                     : theme.palette.selected.backgroundText
            }

            /* UITK 1.3 specs: Slot C */
            Label {
                SlotsLayout.position: SlotsLayout.Trailing

                text: model.index + 1
                color: (loView.document.currentPart === model.index) ? UbuntuColors.orange
                                                                     : theme.palette.selected.backgroundText
            }
        }
    }

    Scrollbar { flickableItem: view; parent: view.parent }
}
