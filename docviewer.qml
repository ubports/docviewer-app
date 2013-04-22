import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.ListItems 0.1 as ListItem

MainView {
    // objectName for functional testing purposes (autopilot-qt5)
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
            
            title: getNameOfFile(file);
            
            page: Page {

                tools: ToolbarActions {
                    back {
                        visible: true
                    }
                }

                Column {
                    id: columnMain;
                    width: parent.width;
                    height: childrenRect.height;

                    TextArea {

                        id: textAreaMain;

                        width: parent.width;
                        height: units.gu(66);

                        activeFocusOnPress: false;
                        highlighted: true;

                        signal loadCompleted()

                        Component.onCompleted: {
                            var xhr = new XMLHttpRequest;

                            xhr.open("GET", file);
                            xhr.onreadystatechange = function() {
                                if (xhr.readyState === XMLHttpRequest.DONE) {
                                    textAreaMain.text = xhr.responseText;
                                    textAreaMain.loadCompleted();
                                }
                            }

                            xhr.send();
                        }
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
                        value: file
                    }
                    ListItem.SingleValue {
                        text: i18n.tr("Size")
                        value: printSize(textAreaMain.text.length);
                    }

                    ListItem.SingleValue {
                        text: i18n.tr("Created")
                        value: "01/01/2001";
                    }

                    ListItem.SingleValue {
                        text: i18n.tr("Modified")
                        value: "01/01/2001";
                    }
                }
            }
        }
    }

    function printSize(size)
    {
        if (size >= 1073741824)
        {
            return parseInt(size/1073741824) + "," + size%1073741824 + " Gio";
        }

        if (size >= 1048576)
        {
            return parseInt(size/1048576) + "," + size%1048576 + " Mio";
        }

        if (size >= 1024)
        {
            return parseInt(size/1024) + "," + size%1024 + " Kio";
        }

        return size + " o";
    }

    function getNameOfFile(path)
    {
        var name = String(path);

        return name.substring(name.lastIndexOf('/')+1);

    }
}
