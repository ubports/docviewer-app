import QtQuick 2.0
import Ubuntu.Components 0.1

Flickable {
    id: flickable
    anchors.fill: parent
    contentHeight: image.height
    contentWidth: parent.width

    Image{
        id: image

        source: file.getPath()
        smooth: true
        fillMode: Image.PreserveAspectFit
        width: parent.width
    }
}
