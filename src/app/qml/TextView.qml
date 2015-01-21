import QtQuick 2.3
import Ubuntu.Components 1.1
import Ubuntu.Components.Themes.Ambiance 0.1

import "utils.js" as Utils

Page {
    id: textPage
    title: Utils.getNameOfFile(file.path);

    Scrollbar {
        flickableItem: flickable
    }

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

    // *** HEADER ***
    state: "default"
    states: [
        ImageViewDefaultHeader {
            name: "default"
            targetPage: textPage
        }
    ]
}
