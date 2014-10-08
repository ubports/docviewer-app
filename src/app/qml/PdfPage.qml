import QtQuick 2.0
import Ubuntu.Components 0.1


Image
{

    property double fixedWidth

    id: imagePage
    asynchronous: true
    width: parent.width
    sourceSize.width: fixedWidth
    fillMode: Image.PreserveAspectCrop
    source: "image://poppler/page/1"

    Connections {
        target: parent

        onShouldReloadImgChanged: {
            if (parent.shouldReloadImg)
            {
                fixedWidth = parent.width
            }
        }
    }

    Component.onCompleted: {
        fixedWidth = parent.width
    }
}
