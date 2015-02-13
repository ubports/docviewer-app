/*
  Copyright (C) 2015 Stefano Verzegnassi

    This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License 3 as published by
  the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
  along with this program. If not, see http://www.gnu.org/licenses/.
*/

import QtQuick 2.0
import Ubuntu.Components 1.1
import QtQuick.Layouts 1.1

import "../common/utils.js" as Utils
import "../upstreamComponents"

ListItemWithActions {
    function formattedDateTime() {
        var date = new Date(model.date)
        var diff = model.dateDiff

        if (diff < 2)
            return Qt.formatDateTime(date, "hh:mm")

        if (diff < 7)
            return Qt.formatDateTime(date, "dddd, hh:mm")

        return Qt.formatDateTime(date, "dd-MM-yyyy hh:mm")
    }

    width: parent.width
    height: units.gu(8)


    locked: documentPage.state == "pickMode"

    // TODO: NEEDS-DESIGN: Enable left action. Still need to find an equivalent for GridDelegate.
   /* leftSideAction: Action {
        iconName: "delete"
        text: i18n.tr("Delete")
        onTriggered: {
            PopupUtils.open(Qt.resolvedUrl("DeleteFileDialog.qml"),
                            documentPage, { path: model.filePath })
        }
    }*/

    contents: RowLayout {
        anchors.fill: parent
        spacing: units.gu(2)

        Icon {
            width: height
            height: units.gu(5)

            // At the moment the suru icon theme doesn't have much icons.
            name: {
                if (model.mimetype.substring(0, 5) === "text/")
                    return "text-x-generic-symbolic"

                if (model.mimetype.substring(0, 5) === "image")
                    return "image-x-generic-symbolic"

                if (model.mimetype === "application/pdf")
                    return "application-pdf-symbolic"

                return "package-x-generic-symbolic"
            }
        }

        Column {
            Layout.fillWidth: true

            Label {
                text: model.name
                wrapMode: Text.Wrap
                width: parent.width

                color: UbuntuColors.midAubergine
            }

            RowLayout {
                width: parent.width

                Label {
                    text: formattedDateTime()
                    fontSize: "small"

                    Layout.fillWidth: true
                }

                Label {
                    text: Utils.printSize(model.size)
                    fontSize: "small"
                }
            }
        }
    }
}
