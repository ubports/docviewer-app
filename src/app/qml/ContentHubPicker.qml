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

    head.sections.model: [i18n.tr("Documents"), i18n.tr("Pictures"), i18n.tr("Other")]
    head.backAction: Action {
        iconName: "back"
        text: i18n.tr("Back")
        onTriggered: pageStack.pop()
    }

    ContentPeerPicker {
        // Do not show ContentPeerPicker header, since we need head.sections.
        showTitle: false

        contentType: {
            switch (picker.head.sections.selectedIndex) {
            case 0:
                return ContentType.Documents
            case 1:
                return ContentType.Pictures
            case 2:
                return ContentType.Unknown
            }
        }
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
