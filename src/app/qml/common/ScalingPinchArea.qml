import QtQuick 2.4

PinchArea {
    id: pinchArea

    property var targetFlickable: null
    property real totalScale: 1

    onPinchStarted: {
        targetFlickable.interactive = false
    }

    onPinchUpdated: {
        pinchUpdatedHandler(pinch)
    }
    onPinchFinished: {
        targetFlickable.interactive = true
        pinchFinishedHandler()
    }

    // ------------------------ Desktop DEBUG

//    MouseArea {
//        id: testMa
//        anchors.fill: parent
//        visible: Qt.platform.os == "linux"
//        z: 19
//        acceptedButtons: Qt.RightButton

//        property int touchPointY
//        property int touchPointX
//        onPressed: {
//            touchPointY = mouse.y
//            touchPointX = mouse.x
//        }
//        onPositionChanged: {
//            var sc = (1 + (mouse.y - touchPointY) / 200)
//            pinchUpdatedHandler({"center" : { "x" : mouse.x, "y" : mouse.y }, "scale" : sc })
//        }
//        onReleased: {
//            pinchFinishedHandler()
//        }
//    }

    // ------------------------ Desktop DEBUG end

    function pinchUpdatedHandler(pinch) {
        targetFlickable.scale = pinch.scale
    }

    function pinchFinishedHandler() {
        var pt = pinchArea.mapFromItem(targetFlickable, -targetFlickable.contentX , -targetFlickable.contentY )
        // console.log("pinchFinishedHandler", -myItem.contentX, -myItem.contentY, Math.round(pt.x), Math.round(pt.y))

        totalScale = targetFlickable.scale * totalScale
        targetFlickable.scale = 1

        // Overwrite contentX and contentY values.
        // This is required since a change in contentWidth or contentHeight causes
        // the Flickable to reset the position of the its content.

        if (targetFlickable.contentWidth > targetFlickable.width)
            targetFlickable.contentX = -pt.x

        if (targetFlickable.contentHeight > targetFlickable.height)
            targetFlickable.contentY = -pt.y

        // Return to the legal bounds
        targetFlickable.returnToBounds()
    }
}
