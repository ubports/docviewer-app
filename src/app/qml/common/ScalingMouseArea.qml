import QtQuick 2.4

MouseArea {
    id: mouseArea
    property Flickable targetFlickable: null
    property real totalScale: 1

    property real minimumZoom: 0.5
    property real thresholdZoom: 1.0
    property real maximumZoom: 4.0
    property int zoomDuration: 100

    property real zoomValue: 1

    onDoubleClicked: {
        var flick = targetFlickable

        var tpt = mouseArea.mapToItem(flick, mouse.x, mouse.y )
        tpt.x -= flick.width * 0.5
        tpt.y -= flick.height * 0.5

        var pt = mouseArea.mapToItem(flick.contentItem, mouse.x, mouse.y)

        scaleInfo.fromZoom = zoomValue

        if (zoomValue <= thresholdZoom)
            scaleInfo.toZoom = maximumZoom
        else
            scaleInfo.toZoom = minimumZoom

        scaleInfo.tempContentX = tpt.x * scaleInfo.effectiveZoom - (flick.width * 0.5)
        scaleInfo.tempContentY = tpt.y * scaleInfo.effectiveZoom - (flick.height * 0.5)

        scaleInfo.finalContentX = Math.max(0, Math.min(pt.x * scaleInfo.effectiveZoom - (flick.width * 0.5),
                                                       (flick.contentWidth * scaleInfo.effectiveZoom) - flick.width))

        scaleInfo.finalContentY = Math.max(0, Math.min(pt.y * scaleInfo.effectiveZoom - (flick.height * 0.5),
                                                       (flick.contentHeight * scaleInfo.effectiveZoom) - flick.height))

        if (zoomValue <= thresholdZoom)
            zoomInAnimation.start()
        else
            zoomOutAnimation.start()
    }

    QtObject {
        id: scaleInfo

        property real fromZoom: 1.0
        property real toZoom: 1.0
        property real effectiveZoom: toZoom / fromZoom

        property int tempContentX: 0
        property int tempContentY: 0

        property int finalContentX: 0
        property int finalContentY: 0
    }

    SequentialAnimation {
        id: zoomInAnimation
        ScriptAction { script: targetFlickable.interactive = false; }

        // Fake zooming
        ParallelAnimation {
            NumberAnimation {
                target: targetFlickable.contentItem
                property: "scale"
                duration: mouseArea.zoomDuration
                to: scaleInfo.effectiveZoom
            }
            NumberAnimation {
                target: targetFlickable
                property: "contentX"
                duration: mouseArea.zoomDuration
                to: scaleInfo.tempContentX
            }
            NumberAnimation {
                target: targetFlickable
                property: "contentY"
                duration: mouseArea.zoomDuration
                to: scaleInfo.tempContentY
            }
        }

        ScriptAction { script: targetFlickable.contentItem.scale = 1; }
        ScriptAction { script: totalScale = scaleInfo.toZoom; }
        ScriptAction { script: targetFlickable.contentX = scaleInfo.finalContentX; }
        ScriptAction { script: targetFlickable.contentY = scaleInfo.finalContentY; }
        ScriptAction { script: targetFlickable.returnToBounds(); }
        ScriptAction { script: targetFlickable.interactive = true; }
    }

    SequentialAnimation {
        id: zoomOutAnimation

        ScriptAction { script: targetFlickable.interactive = false; }
        ScriptAction { script: totalScale = scaleInfo.toZoom; }
        ScriptAction { script: targetFlickable.contentX = scaleInfo.finalContentX; }
        ScriptAction { script: targetFlickable.contentY = scaleInfo.finalContentY; }
        ScriptAction { script: targetFlickable.returnToBounds(); }
        ScriptAction { script: targetFlickable.interactive = true; }
    }
}
