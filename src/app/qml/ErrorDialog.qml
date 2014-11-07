import QtQuick 2.3
import Ubuntu.Components 1.1
import Ubuntu.Components.Popups 1.0

Dialog {
    id: errorDialogue
    title: i18n.tr("Error")
    text: i18n.tr("File does not exist")

    Button {
        text: i18n.tr("Close")
        color: "red"

        onClicked: Qt.quit();
    }
}
