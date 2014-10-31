import QtQuick 2.3
import Ubuntu.Components 1.1

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

    Flickable {
        id: flickImg
        anchors.fill: parent
        contentHeight: image.height

        PinchArea {
            id: pinchy
            anchors.fill: image
            enabled: true

            property real lastWidth
            property real lastHeight
            property double p1toC_X
            property double p1toC_Y
            property double contentInitX
            property double contentInitY

            onPinchStarted: {
                lastWidth = image.width
                lastHeight = image.height

                contentInitX = flickImg.contentX
                contentInitY = flickImg.contentY

            }

            onPinchUpdated: {

                var newWidth = 0;

                newWidth = lastWidth*pinch.scale;

                if (newWidth < image.startWidth)
                    newWidth = image.startWidth;
                else if (newWidth > image.sourceSize.width)
                    newWidth = image.sourceSize.width;

                flickImg.contentWidth = newWidth;

                flickImg.contentX = contentInitX-(lastWidth-newWidth)/2
                flickImg.contentY = contentInitY-(lastHeight-image.height)/2

            }
        }

        Image {

            property real startWidth

            id: image
            objectName: "imageItem"

            source: file.path
            smooth: true
            fillMode: Image.PreserveAspectFit
            width: parent.width

            Component.onCompleted: {
                if (width > sourceSize.width)
                    startWidth = sourceSize.width
                else
                    startWidth = width


            }
        }

    }
}
