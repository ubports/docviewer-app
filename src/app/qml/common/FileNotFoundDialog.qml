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
import Ubuntu.Components 1.2
import Ubuntu.Components.Popups 1.0

// We may want to refactor this dialog for a more generic usage, when we'll need it.
Dialog {
    id: errorDialog
    title: i18n.tr("Error")
    text: i18n.tr("File does not exist")

    Button {
        text: i18n.tr("Close")
        color: UbuntuColors.red
        onClicked: PopupUtils.close(errorDialog)
    }
}
