import QtQuick 2.0
import Ubuntu.Components 1.1

Page {
    id: welcomePage

    title: i18n.tr("Document Viewer")
    head.actions: [ openAction ]

    EmptyState {
        title: i18n.tr("No opened documents")
        subTitle: i18n.tr("Tap the + icon to open a document")

        iconName: "edit-copy"

        anchors.centerIn: parent
    }

    Action {
        id: openAction
        text: i18n.tr("Open a file...")
        iconName: "add"

        onTriggered: pageStack.push(Qt.resolvedUrl("ContentHubPicker.qml"))
    }
}
