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

PageHeadState {
    id: rootItem

    property Page targetPage
    head: targetPage.head

    contents: Item {
        anchors.fill: parent

        ZoomSelector {
            width: units.gu(24)
            anchors {
                right: parent.right
                verticalCenter: parent.verticalCenter
            }
        }
    }

    backAction: Action {
        iconName: "close"
        text: i18n.tr("Hide zoom controls")
        onTriggered: targetPage.state = "default"
    }

    actions: [
        Action {
            iconName: "zoom-in"
            text: i18n.tr("Zoom in")
            onTriggered: loView.zoomFactor += 0.1
        },

        Action {
            iconName: "zoom-out"
            text: i18n.tr("Zoom out")
            onTriggered: loView.zoomFactor -= 0.1
        }
    ]
}
