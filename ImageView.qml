import QtQuick 2.0

    Image{
        id: image

        source: file.getPath()
        smooth: true
        fillMode: Image.PreserveAspectFit
        width: parent.width
    }
