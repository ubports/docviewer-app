import QtQuick 2.4

PinchArea {
    id: pinchArea

    property Flickable targetFlickable: null
    property real totalScale: 1

    property real minimumZoom: 0.5
    property real maximumZoom: 4.0
    property alias zoomValue: zoomHelper.scale

    function pinchUpdatedHandler(pinch) {
        if (zoomHelper.scale < pinchArea.maximumZoom &&
                zoomHelper.scale > pinchArea.minimumZoom) {
            targetFlickable.scale = pinch.scale
        }
    }

    function pinchFinishedHandler() {
        var pt = pinchArea.mapFromItem(targetFlickable, -targetFlickable.contentX , -targetFlickable.contentY )
        // console.log("pinchFinishedHandler", -myItem.contentX, -myItem.contentY, Math.round(pt.x), Math.round(pt.y))

        totalScale = zoomHelper.scale
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

    pinch {
        target: Item { id: zoomHelper }
        minimumScale: pinchArea.minimumZoom
        maximumScale: pinchArea.maximumZoom
    }

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
}
