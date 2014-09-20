import QtQuick 2.3
import Ubuntu.Components 0.1


Flickable {
    id: flickable
    anchors.fill: parent
    contentHeight: columnMain.height
    contentWidth: parent.width


    Column {
        id: columnMain
		objectName: "columnMain"
        width: parent.width
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

