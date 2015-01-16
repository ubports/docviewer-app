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

    File {
        objectName: "fileObject"
        id: file

        onMimetypeChanged: LoadComponent.load(mimetype)
        onErrorChanged: { if (error == -1); PopupUtils.open(errorDialog) }
    }

    Component.onCompleted: {
        // Check if a value has been specified for "documentPath" argument.
        // The value for the argument is parsed in main.cpp.

        if (documentPath) {
            // If so, send the path to the File plugin and load the document.
            console.log("Path argument is:", documentPath);
            file.path = documentPath;
        } else {
            // Otherwise, push a welcome screen in the stack.
            pageStack.push(Qt.resolvedUrl("WelcomePage.qml"));
        }
    }

    // Content Importer
    // Used when user asks to open a document from ContentHub.
    Loader {
        id: contentHubLoader

        asynchronous: true
        source: Qt.resolvedUrl("ContentHubProxy.qml")
        onStatusChanged: {
            if (status === Loader.Ready) {
                item.pageStack = pageStack
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
