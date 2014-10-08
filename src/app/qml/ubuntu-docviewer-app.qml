import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.Popups 0.1
import Ubuntu.Components.ListItems 0.1 as ListItem

import org.docviewer.file 1.0

import "utils.js" as Utils
import "loadComponent.js" as LoadComponent

MainView {
    objectName: "docviewer"

    applicationName: "com.ubuntu.docviewer"
    
    width: units.gu(50)
    height: units.gu(75)

    Component.onCompleted: {
    }

    Component.onDestruction: {

    }

    Arguments {
        id: args

        defaultArgument.help: "Path of the document"
        defaultArgument.valueNames: ["path"]
    }

    File {
        id: file
        path: Utils.relToAbs(args.defaultArgument.at(0))

        onMimetypeChanged: {
            var mimetypeValue = file.mimetype;
            mimetypeItem.value = LoadComponent.load(mimetypeValue);
        }
    }
    
    Tabs {
        id: tabs

        Tab {
            objectName: "tabViewer"
            id: tabViewer;
            anchors.fill: parent

            
            title: Utils.getNameOfFile(file.path);

            page: Page {
                id: pageMain

                anchors.fill: parent

                tools: ToolbarItems {
                    back {
                        visible: true
                    }
                }


            }
        }

        Tab {
            objectName: "TabDetails"
            id: tabDetails;

            title: i18n.tr("Details")
            page: Page {
                Column {
                    width: parent.width

                    ListItem.Subtitled {
                        text: i18n.tr("Location")
                        subText: file.path
                    }
                    ListItem.SingleValue {
                        text: i18n.tr("Size")
                        value: Utils.printSize(file.size)
                    }

                    ListItem.Subtitled {
                        text: i18n.tr("Created")
                        subText: qsTr("%1").arg(file.creationTime.toLocaleString(Qt.locale()))
                    }

                    ListItem.Subtitled {
                        text: i18n.tr("Last modified")
                        subText: qsTr("%1").arg(file.lastModified.toLocaleString(Qt.locale()))
                    }

                    ListItem.SingleValue {
                        id: mimetypeItem
						objectName: "mimetypeItem"
                        text: i18n.tr("MIME type")
                        value: "none"
                    }
                }
            }
        }
    }

    Component {
        id: unknownTypeDialog
        UnknowTypeDialog {

        }
    }

    function runUnknownTypeDialog()
    {
        PopupUtils.open(unknownTypeDialog, pageMain);
    }
}
