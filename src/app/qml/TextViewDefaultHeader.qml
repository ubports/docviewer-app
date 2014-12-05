import QtQuick 2.3
import Ubuntu.Components 1.1
import Ubuntu.Components.Popups 1.0

PageHeadState {
    id: rootItem

    property Page targetPage

    head: targetPage.head

    contents: Column {
        anchors.centerIn: parent
        width: parent.width

        Label {
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            elide: Text.ElideMiddle

            font.weight: Font.DemiBold
            text: targetPage.title
        }
    }

    backAction: Action {
        iconName: "back"
        text: (pageStack.depth > 1) ? i18n.tr("Back") : i18n.tr("Close")
        onTriggered: {
            if (pageStack.depth > 1) {
                // Go back to Welcome page
                pageStack.pop();
            } else {
                // File has been imported through Content Hub (or was not chosen through WelcomePage)
                // Close the application and show our source app (e.g. ubuntu-filemanager-app, if used to open a document)
                Qt.quit()
            }
        }
    }

    actions: [
        Action {
            objectName: "detailsAction"
            text: i18n.tr("Details")
            iconName: "info"
            onTriggered: pageStack.push(Qt.resolvedUrl("DetailsPage.qml"))
        }
    ]
}
