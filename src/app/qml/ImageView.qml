import QtQuick 2.3
import Ubuntu.Components 1.1

import "utils.js" as Utils

Page {
    id: imagePage
    title: Utils.getNameOfFile(file.path);

    ZoomableImage {
        anchors.fill: parent

        zoomable: true
        source: file.path
    }

    // *** HEADER ***
    state: "default"
    states: [
        ImageViewDefaultHeader {
            name: "default"
            targetPage: imagePage
        }
    ]
}
