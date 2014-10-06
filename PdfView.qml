import QtQuick 2.3
import Ubuntu.Components 1.1
import org.docviewer.poppler 1.0

Flickable {
    id: flickable
    anchors.fill: parent

    contentHeight: columnPages.height + 10
    contentWidth: parent.width

    PinchArea {
        id: pinchy
        anchors.fill: columnPages
        enabled: true
        pinch.target: flickable

        property real lastWidth
        /*property real lastHeight
        property double p1toC_X
        property double p1toC_Y
        property double contentInitX
        property double contentInitY*/

        onPinchStarted: {
            lastWidth = flickable.width

            /*contentInitX = flickImg.contentX
            contentInitY = flickImg.contentY*/

        }

        onPinchUpdated: {

            var newWidth = 0;

            newWidth = lastWidth*pinch.scale;

            /*if (newWidth < image.startWidth)
                newWidth = image.startWidth;
            else if (newWidth > image.sourceSize.width)
                newWidth = image.sourceSize.width;*/

            flickable.contentWidth = newWidth;

            /*flickImg.contentX = contentInitX-(lastWidth-newWidth)/2
            flickImg.contentY = contentInitY-(lastHeight-image.height)/2*/

        }

        onPinchFinished: {

            columnPages.shouldReloadImg = true;
            console.log("FINISHED");
        }
    }


    Poppler {
        id: popplerProp
        path: file.path
    }

    Column {
        id: columnPages

        property bool shouldReloadImg : false

        width: parent.width - 10
        x: 5
        y: 5
        spacing: 10

        onWidthChanged: {
            if (!pinchy.pinch.active)
                shouldReloadImg=true
            else
                shouldReloadImg=false
        }

    }

    Component.onCompleted: {

        var i=0;
        for(i=1; i <= popplerProp.numPages; i++)
        {
            var component = Qt.createComponent("PdfPage.qml");

            if (component.status === Component.Error)
            {
                console.debug("Error creating component");
            }
            else
            {
                var page = component.createObject(columnPages);

                page.source = "image://poppler/page/"+i;
            }
        }
    }
}
