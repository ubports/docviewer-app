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

import QtQuick 2.3
import Ubuntu.Components 1.1
import Ubuntu.Components.ListItems 1.0 as ListItems
 
Panel {
    id: bottomPanel
    anchors {
        left: parent.left
        bottom: parent.bottom
        right: parent.right
    }
 
    readonly property bool shouldBeVisible: loPage.header.y > -1
    onShouldBeVisibleChanged: {
        if (shouldBeVisible)
            bottomPanel.open()
        else
            bottomPanel.close()
    }
 
    height: units.gu(6)
 
    Rectangle {
        anchors.fill: parent
        color: "white"
 
        ListItems.ThinDivider {
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
           }
        }
    }
}
