import QtQuick 2.0
import Ubuntu.Components 0.1
import org.docviewer.poppler 1.0

Flickable {
    id: flickable
    anchors.fill: parent

    contentHeight: columnPages.height + 10
    contentWidth: parent.width

    Poppler {
        id: popplerProp
        path: file.getPath()
    }

    Column {
        id: columnPages

        width: parent.width - 10
        x: 5
        y: 5
        spacing: 10

    }

    Component.onCompleted : {

        var i=0;
        for(i=1; i <= popplerProp.numPages; i++)
        {
            var component = Qt.createComponent("PdfPage.qml");

            if (component.status === Component.Error)
            {
                console.debug("Error creating component");
            }
            else
            {
                var page = component.createObject(columnPages);

                page.source = "image://poppler/page/"+i;
            }
        }
    }
}
