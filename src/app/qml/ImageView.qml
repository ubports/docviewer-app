import QtQuick 2.3
import Ubuntu.Components 1.1

import "utils.js" as Utils

Page {
    id: pageMain
    title: Utils.getNameOfFile(file.path);

    head.actions: [
        Action {
            objectName: "detailsAction"
            text: i18n.tr("Details")
            iconName: "info"
            onTriggered: pageStack.push(Qt.resolvedUrl("DetailsPage.qml"))
        }
    ]

    ZoomableImage {
        anchors.fill: parent

        zoomable: true
        source: file.path
    }
}
