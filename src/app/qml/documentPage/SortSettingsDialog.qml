/*
  Copyright (C) 2013-2015 Stefano Verzegnassi

    This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License 3 as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

    This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
  along with this program. If not, see http://www.gnu.org/licenses/.
*/

import QtQuick 2.4
import Ubuntu.Components 1.2
import Ubuntu.Components.Popups 1.0
import QtQuick.Layouts 1.1

Dialog {
    id: sortSettingsDialog

    title: i18n.tr("Sorting settings")

    OptionSelector {
        expanded: true
        model: [
            i18n.tr("Sort by date (Latest first)"),
            i18n.tr("Sort by name (A-Z)"),
            i18n.tr("Sort by size (Smaller first)")
        ]

        Component.onCompleted: selectedIndex = sortSettings.sortMode
        onSelectedIndexChanged: sortSettings.sortMode = selectedIndex
    }

    RowLayout {
        CheckBox {
            Component.onCompleted: checked = sortSettings.reverseOrder
            onCheckedChanged: sortSettings.reverseOrder = checked
        }

        Label {
            text: i18n.tr("Reverse order")
            Layout.fillWidth: true
        }
    }

    Button {
        text: i18n.tr("Close")
        color: UbuntuColors.green
        onClicked: PopupUtils.close(sortSettingsDialog)
    }
}

