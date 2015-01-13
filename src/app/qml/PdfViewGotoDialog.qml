import QtQuick 2.3
import Ubuntu.Components 1.1
import Ubuntu.Components.Popups 1.0

Dialog {
    id: goToPageDialogue
    objectName:"PdfViewGotoDialog"

    property ListView view

    title: i18n.tr("Go to page")
    text: i18n.tr("Choose a page between 1 and %1").arg(pdfView.count)

    TextField {
        id: goToPageTextField
        objectName:"goToPageTextField"

        width: parent.width

        hasClearButton: true
        inputMethodHints: Qt.ImhFormattedNumbersOnly
        validator: IntValidator{ bottom: 1; top: view.count }

        Keys.onReturnPressed: goToPage()
        Component.onCompleted: forceActiveFocus()
    }

    Button {
        objectName:"GOButton"
        text: i18n.tr("GO!")
        color: UbuntuColors.orange

        enabled: goToPageTextField.acceptableInput
        onClicked: goToPage()
    }

    Button {
        text: i18n.tr("Cancel")
        onClicked: PopupUtils.close(goToPageDialogue)
    }

    function goToPage() {
        view.positionViewAtIndex((goToPageTextField.text - 1), ListView.Beginning)
        PopupUtils.close(goToPageDialogue)
    }
}
