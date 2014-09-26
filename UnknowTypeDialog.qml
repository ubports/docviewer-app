import QtQuick 2.3
import Ubuntu.Components 0.1
import Ubuntu.Components.Popups 0.1

import "loadComponent.js" as LoadComponent

Dialog {
    id: unknownDialog
    objectName: "unknownDialog"
    title: i18n.tr("Unknown file type")
    text: i18n.tr("Sorry but we can't find a way to display this file. Do you want to open it as a plain text?")
    Button {
        text: i18n.tr("Yes")
        color: "green"

        onClicked: {
            LoadComponent.load("text/plain");
            unknownDialog.destroy();
        }
    }
    Button {
        text: i18n.tr("No")
        color: "red"

        onClicked: {
            unknownDialog.destroy();
        }
    }
}

