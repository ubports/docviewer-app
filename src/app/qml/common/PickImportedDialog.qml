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

import QtQuick 2.4
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.0

import "utils.js" as Utils

Dialog {
    id: multipleImportDialog

    property alias model: repeater.model

    title: i18n.tr("Multiple documents imported")
    text: i18n.tr("Choose which one to open:")

    // We don't use a Flickable, since the Dialog has already one.
    Repeater {
        id: repeater

        ListItem {
            onClicked: {
                PopupUtils.close(multipleImportDialog);
                mainView.openDocument(model.path);
            }

            Label {
                text: Utils.getNameOfFile(model.path)
                anchors {
                    left: parent.left; right: parent.right
                    margins: units.gu(2)
                    verticalCenter: parent.verticalCenter
                }
            }
        }
    }

    Button {
        text: i18n.tr("Close")
        onClicked: PopupUtils.close(multipleImportDialog)
    }
}
