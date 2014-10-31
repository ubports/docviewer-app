import QtQuick 2.3
import Ubuntu.Components 1.1
import Ubuntu.Components.Themes.Ambiance 0.1

import "utils.js" as Utils

Page {
    id: pageMain
    title: Utils.getNameOfFile(file.path);

    head.actions: [
        Action {
            objectName: "detailsAction"
            text: i18n.tr("Details")
            iconName: "info"
            onTriggered: pageStack.push(Qt.resolvedUrl("DetailsPage.qml"))
        }
    ]

    Scrollbar {
        flickableItem: flickable
    }

    Flickable {
        id: flickable
        anchors.fill: parent
        contentHeight: columnMain.height
        contentWidth: parent.width

        boundsBehavior: Flickable.StopAtBounds

        Column {
            id: columnMain
            objectName: "columnMain"

            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
            }

            spacing: 10

            TextArea {
                id: textAreaMain
                objectName: "textAreaMain"

                width: parent.width

                autoSize: true
                maximumLineCount: 0
                readOnly: true
                //activeFocusOnPress: true;
                highlighted: true

                text: "Loading..."

                // Use Ubuntu Mono font so we can know how many chars are placed in a single line.
                font.family: "UbuntuMono"

                property int maxCharsPerLine: contentWidth / (font.pixelSize * 0.5)
                // Ubuntu Mono font ratio is 1:2 (width:height)

                signal loadCompleted()

                Component.onCompleted: {
                    var xhr = new XMLHttpRequest;

                    xhr.open("GET", file.path);
                    xhr.onreadystatechange = function() {
                        if (xhr.readyState === XMLHttpRequest.DONE) {
                            textAreaMain.text = xhr.responseText;
                            textAreaMain.loadCompleted();
                        }
                    }

                    xhr.send();
                    flickable.forceActiveFocus()
                }

                style: TextAreaStyle {
                    background: Item {
                        id: bgItem
                        anchors.fill: parent
                    }
                }
            }
        }

        Keys.onPressed: {
            if (event.key === Qt.Key_PageDown) {
                if (!flickable.atYEnd)
                    flickable.contentY = flickable.contentY + height / 2;
                event.accepted = true;
            } else if (event.key === Qt.Key_PageUp) {
                if (!flickable.atYBeginning)
                    flickable.contentY = flickable.contentY - height / 2;
                event.accepted = true;
            }
        }
        Keys.onLeftPressed: {
            flickable.flick( -width / 2, 0)
        }
        Keys.onRightPressed: {
            flickable.flick( width / 2, 0)
        }
        Keys.onUpPressed: {
            flickable.flick( 0, height / 2)
        }
        Keys.onDownPressed: {
            flickable.flick( 0, -height / 2)
        }
    }
}
