import QtQuick 2.3
import Ubuntu.Components 1.1
import Ubuntu.Components.Themes.Ambiance 0.1

import "utils.js" as Utils

Page {
    id: pageMain
    title: Utils.getNameOfFile(file.path);

    head.backAction: Action {
        iconName: "back"
        text: (pageStack.depth > 1) ? i18n.tr("Back") : i18n.tr("Close")
        onTriggered: {
            if (pageStack.depth > 1) {
                // Go back to Welcome page
                pageStack.pop();
            } else {
                // File has been imported through Content Hub (or was not chosen through WelcomePage)
                // Close the application and show our source app (e.g. ubuntu-filemanager-app if used to open a document)
                Qt.quit()
            }
        }
    }

    head.actions: [
        Action {
            objectName: "detailsAction"
            text: i18n.tr("Details")
            iconName: "info"
            onTriggered: pageStack.push(Qt.resolvedUrl("DetailsPage.qml"))
        }
    ]

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
            }

            xhr.send();
        }

        style: TextAreaStyle {
            background: Rectangle { color: "white" }
        }
    }
}
