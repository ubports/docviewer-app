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

import QtQuick 2.0
import Ubuntu.Components 1.1
import Ubuntu.Content 1.1

Page {
    id: picker
    title: i18n.tr("Open with...")

    property var activeTransfer

    ContentPeerPicker {
        contentType: ContentType.Documents
        handler: ContentHandler.Source

        onPeerSelected: picker.activeTransfer = peer.request();
    }

    ContentTransferHint {
        id: transferHint
        anchors.fill: parent
        activeTransfer: picker.activeTransfer
    }

    Connections {
        target: picker.activeTransfer ? picker.activeTransfer : null
        onStateChanged: {
            if (picker.activeTransfer.state === ContentTransfer.Charged) {
                // Close ContentHubPicker page.
                pageStack.pop();

                file.path = picker.activeTransfer.items[0].url.toString().replace("file://", "")
                console.log("[CONTENT-HUB] Content imported!")
            }
        }
    }
}
