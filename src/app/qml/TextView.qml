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

import QtQuick 2.3
import Ubuntu.Components 1.1
import Ubuntu.Components.Themes.Ambiance 0.1

import "utils.js" as Utils

Page {
    id: textPage
    title: Utils.getNameOfFile(file.path);

    TextArea {
        id: textAreaMain
        objectName: "textAreaMain"

        anchors.fill: parent

        // FIXME: If set to true, some of the keyboard hooks are disabled
        // And it's not possible to move the cursor with arrow keys.
        readOnly: true

        text: i18n.tr("Loading...")
        font.family: "UbuntuMono"

        Component.onCompleted: {
            var xhr = new XMLHttpRequest;

            xhr.open("GET", file.path);
            xhr.onreadystatechange = function() {
                if (xhr.readyState === XMLHttpRequest.DONE) {
                    textAreaMain.text = xhr.responseText;
                }
            };

            xhr.send();
        }

        style: TextAreaStyle {
            background: Rectangle { color: "white" }
        }
    }

    // *** HEADER ***
    state: "default"
    states: [
        TextViewDefaultHeader {
            name: "default"
            targetPage: textPage
        }
    ]
}
