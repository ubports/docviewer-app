import QtQuick 2.4

PinchArea {
    id: pinchArea

    property var targetFlickable: null
    property real totalScale: 1

    onPinchUpdated: {
        pinchUpdatedHandler(pinch)
    }
    onPinchFinished: {
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
        targetFlickable.contentX = -pt.x
        targetFlickable.contentY = -pt.y

        totalScale = targetFlickable.scale * totalScale
        targetFlickable.scale = 1
    }
}
