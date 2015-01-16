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
