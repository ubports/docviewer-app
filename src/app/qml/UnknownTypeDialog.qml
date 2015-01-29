/*
 * Copyright (C) 2013 Canonical, Ltd.
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

import "loadComponent.js" as LoadComponent

Dialog {
    id: unknownDialog
    objectName: "unknownDialog"
    title: i18n.tr("Unknown file type")
    text: i18n.tr("Sorry but we can't find a way to display this file. Do you want to open it as a plain text?")
    Button {
        text: i18n.tr("Yes")
        color: "green"

        onClicked: {
            LoadComponent.load("text/plain");
            unknownDialog.destroy();
        }
    }
    Button {
        text: i18n.tr("No")
        color: "red"

        onClicked: {
            unknownDialog.destroy();
        }
    }
}

