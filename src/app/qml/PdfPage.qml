import QtQuick 2.3
import Ubuntu.Components 1.1
import QtGraphicalEffects 1.0

Rectangle {
    id: pdfPage
    width: parent.width

    height: width * (model.height / model.width)

    border {
        width: 1
        color: "#808080"
    }

    Image {
        id: imagePage
        anchors {
            fill: parent
            margins: 1
        }
        asynchronous: true
        sourceSize.width: parent.width - 2
        fillMode: Image.PreserveAspectCrop

        Component.onCompleted: source = "image://poppler/page/" + (model.index + 1)
    }

    Rectangle {
        anchors.fill: parent
        color: "white"
        visible: imagePage.status === Image.Loading

        ActivityIndicator {
            anchors.centerIn: parent
            running: parent.visible
        }
    }

    DropShadow {
        anchors.fill: parent
        cached: true;
        horizontalOffset: 0;
        verticalOffset: 2;
        radius: 8.0;
        samples: 16;
        color: "#80000000";
        smooth: true;
        source: parent;
        z: -10
    }
}
