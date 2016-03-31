/*
  Copyright (C) 2015, 2016 Stefano Verzegnassi

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License 3 as published by
  the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see http://www.gnu.org/licenses/.
*/

import QtQuick 2.4
import Ubuntu.Components 1.3
import Ubuntu.Content 1.3

Page {
    id: sharePage

    property url fileUrl
    property var activeTransfer

    header: PageHeader {
        title: i18n.tr("Share to")
    }

    ContentPeerPicker {
        id: picker

        contentType: ContentType.Documents
        handler: ContentHandler.Share
        showTitle: false

        onPeerSelected: {
            activeTransfer = peer.request();
            activeTransfer.items = [ resultComponent.createObject(sharePage, { "url": fileUrl }) ];
            activeTransfer.state = ContentTransfer.Charged;
            pageStack.pop();
        }

        onCancelPressed: {
            pageStack.pop();
        }
    }

    ContentTransferHint {
        anchors.fill: parent
        activeTransfer: sharePage.activeTransfer
    }

    Component {
        id: resultComponent
        ContentItem {}
    }
}
