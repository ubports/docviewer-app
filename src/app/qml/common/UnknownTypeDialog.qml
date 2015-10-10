/*
 * Copyright (C) 2013-2015 Canonical, Ltd.
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
import Ubuntu.Components 1.2
import Ubuntu.Components.Popups 1.0
import QtQuick.Layouts 1.1

import "loadComponent.js" as LoadComponent

Dialog {
    id: unknownDialog
    objectName: "unknownDialog"
    title: i18n.tr("Unknown file type")
    text: i18n.tr("Sorry but we can't find a way to display this file.\n\nDo you want to open it as a plain text?")

    RowLayout {
        anchors {
            left: parent.left
            right: parent.right
            margins: units.gu(-1)
        }

        Button {
            text: i18n.tr("Cancel")
            onClicked: PopupUtils.close(unknownDialog)
            Layout.fillWidth: true
        }

        Button {
            text: i18n.tr("Yes")
            color: UbuntuColors.green
            Layout.fillWidth: true

            onClicked: {
                LoadComponent.load("text/plain");
                PopupUtils.close(unknownDialog)
            }
        }
    }
}

