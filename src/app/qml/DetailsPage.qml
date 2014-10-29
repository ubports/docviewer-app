import QtQuick 2.3
import Ubuntu.Components 1.1
import Ubuntu.Components.ListItems 1.0 as ListItem

import "utils.js" as Utils

Page {
    id: detailsPage
    title: i18n.tr("Details")

    Column {
        width: parent.width

        ListItem.Subtitled {
            text: i18n.tr("Location")
            subText: file.path
        }
        ListItem.Subtitled {
            text: i18n.tr("Size")
            subText: Utils.printSize(file.size)
        }

        ListItem.Subtitled {
            text: i18n.tr("Created")
            subText: i18n.tr("%1").arg(file.creationTime.toLocaleString(Qt.locale()))
        }

        ListItem.Subtitled {
            text: i18n.tr("Last modified")
            subText: i18n.tr("%1").arg(file.lastModified.toLocaleString(Qt.locale()))
        }

        ListItem.Subtitled {
            id: mimetypeItem
            objectName: "mimetypeItem"
            text: i18n.tr("MIME type")
            subText: mainView.mimetype
        }
    }
}
