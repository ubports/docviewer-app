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

import QtQuick 2.4
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.0
import QtQuick.Layouts 1.1

// TODO: Use page breaks detection, when LibreOfficeKit will support it.

Dialog {
    id: goToPageDialog
    objectName: "LOViewGotoDialog"

    title: i18n.tr("Go to position")
    text: i18n.tr("Choose a position between 1% and 100%")

    TextField {
        id: goToPageTextField
        objectName:"goToPageTextField"

        width: parent.width

        hasClearButton: true
        inputMethodHints: Qt.ImhFormattedNumbersOnly
        validator: IntValidator{ bottom: 1; top: 100 }

        Keys.onReturnPressed: goToPage()
        Component.onCompleted: forceActiveFocus()
    }

    RowLayout {
        anchors {
            left: parent.left
            right: parent.right
            margins: units.gu(-1)
        }

        Button {
            text: i18n.tr("Cancel")
            onClicked: PopupUtils.close(goToPageDialog)
            Layout.fillWidth: true
        }

        Button {
            objectName:"GOButton"
            text: i18n.tr("GO!")
            color: UbuntuColors.green
            Layout.fillWidth: true

            enabled: goToPageTextField.acceptableInput
            onClicked: goToPage()
        }
    }

    function goToPage() {
        var pos = loView.contentHeight * parseInt(goToPageTextField.text) / 100

        loView.contentY = Math.min(pos, (loView.contentHeight - loView.height))
        PopupUtils.close(goToPageDialog)
    }
}
