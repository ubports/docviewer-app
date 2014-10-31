import QtQuick 2.3
import Ubuntu.Components 1.1
import Ubuntu.Content 1.1
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

    Arguments {
        id: args

        // It returns "expected argument" message when not specified a path.
        // It works anyway, but it may be worth to use Argument{} in future
        defaultArgument.help: "Path of the document"
        defaultArgument.valueNames: ["path"]
    }

    File {
        objectName: "fileObject"
        id: file

        onMimetypeChanged: LoadComponent.load(mimetype)
        onErrorChanged: { if (error == -1); PopupUtils.open(errorDialog) }
    }

    Component.onCompleted: {
        // Check if a value has been specified for "path" argument
        if (args.defaultArgument.at(0)) {
            // If so, send the path to the File plugin
            console.log("Path argument is:", args.defaultArgument.at(0))      
            file.path = args.defaultArgument.at(0)
        } else {
            // Otherwise, push a welcome screen in the stack
            pageStack.push(Qt.resolvedUrl("WelcomePage.qml"))
        }
    }

    // Content Importer
    Connections {
        target: ContentHub

        onImportRequested: {
            // We have no signals to know if an import was requested before Component.completed signal
            //  is emitted. So clear the stack when this occurs.
            pageStack.clear()

            if (transfer.state === ContentTransfer.Charged) {
                console.log("[CONTENT-HUB] Incoming Import Request")
                file.path = transfer.items[0].url.toString().replace("file://", "");
            }
        }
    }

    function runUnknownTypeDialog() {
        PopupUtils.open(unknownTypeDialog);
    }

    PageStack { id: pageStack }

    Component { id: errorDialog; ErrorDialog {} }
    Component { id: unknownTypeDialog; UnknownTypeDialog {} }
}
