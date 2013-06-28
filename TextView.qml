import QtQuick 2.0
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
            //activeFocusOnPress: false;
            highlighted: true

            text: "Loading..."

            signal loadCompleted()

            Component.onCompleted: {
                var xhr = new XMLHttpRequest;

                xhr.open("GET", file.getPath());
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

}

