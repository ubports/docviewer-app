import QtQuick 2.3
import Ubuntu.Components 1.1
import Ubuntu.Components.Popups 1.0
import com.ubuntu.fileqmlplugin 1.0

import "loadComponent.js" as LoadComponent

MainView {
    id: mainView
    objectName: "docviewer"

    applicationName: "com.ubuntu.docviewer"
    useDeprecatedToolbar: false
    
    width: units.gu(50)
    height: units.gu(75)

    property string mimetype: "none"

   Arguments {
        id: args

        defaultArgument.help: "Path of the document"
        defaultArgument.valueNames: ["path"]
    }

    File {
        objectName: "fileObject"
        id: file
        path: args.defaultArgument.at(0)

        onMimetypeChanged: mainView.mimetype = LoadComponent.load(file.mimetype);
    }

    PageStack {
        id: pageStack

        Component {
            DetailsPage {
                objectName: "TabDetails"
                id: tabDetails;
            }
        }
    }

    Component {
        id: unknownTypeDialog
        UnknownTypeDialog {}
    }

    function runUnknownTypeDialog()
    {
        PopupUtils.open(unknownTypeDialog);
    }
}
