import QtQuick 2.0
import Ubuntu.Components 0.1

Image
{
    id: imagePage
    asynchronous: true
    width: parent.width
    sourceSize.width: width
    fillMode: Image.PreserveAspectFit
    source: "image://poppler/page/1"
}
