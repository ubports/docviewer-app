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
import Ubuntu.Components.Popups 1.0
import Ubuntu.Components.ListItems 1.0 as ListItem

import "utils.js" as Utils

Dialog {
    id: multipleImportDialog

    property alias model: repeater.model

    title: i18n.tr("Multiple documents imported")
    text: i18n.tr("Choose which one to open:")

    // We don't use a Flickable, since it already lives in the Dialog itself.
    Repeater {
        id: repeater
        ListItem.Standard {
            text: Utils.getNameOfFile(model.path)
            __foregroundColor: Theme.palette.selected.backgroundText

            onClicked: {
                PopupUtils.close(multipleImportDialog);
                mainView.openDocument(model.path);
            }
        }
    }

    Button {
        text: i18n.tr("Close")
        onClicked: PopupUtils.close(multipleImportDialog)
    }
}
