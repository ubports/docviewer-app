import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.Popups 0.1
import Ubuntu.Components.ListItems 0.1 as ListItem

import "utils.js" as Utils
import "loadComponent.js" as LoadComponent

MainView {
    objectName: "docviewer"

    applicationName: "ubuntu-docviewer-app"
    
    width: units.gu(50)
    height: units.gu(75)

    Component.onCompleted: {

    }

    Component.onDestruction: {

    }
    
    Tabs {
        id: tabs
        anchors.fill: parent

        Tab {
            objectName: "tabViewer"
            id: tabViewer;
            
            title: Utils.getNameOfFile(file.getPath());
            
            page: Page {
                id: pageMain

                tools: ToolbarActions {
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
                    width: units.gu(50)
                    ListItem.SingleValue {
                        text: i18n.tr("Location")
                        value: file.getPath()
                    }
                    ListItem.SingleValue {
                        text: i18n.tr("Size")
                        value: Utils.printSize(file.getSize())
                    }

                    ListItem.SingleValue {
                        text: i18n.tr("Last modified")
                        value: qsTr("%1").arg(file.getLastModified().toLocaleString(Qt.locale()))
                    }

                    ListItem.SingleValue {
                        id: mimetypeItem
                        text: i18n.tr("Mimetype")
                        value: file.getMimetype()
                    }

                    Connections {
                        target: file
                        onMimetypeChanged: {
                            mimetypeItem.value = file.getMimetype();
                            LoadComponent.load(file.getMimetype());
                        }
                    }
                }
            }
        }
    }

    Component {
        id: unknowTypeDialog
        UnknowTypeDialog {

        }
    }

    function runUnknowTypeDialog()
    {
        PopupUtils.open(unknowTypeDialog, pageMain);
    }
}
