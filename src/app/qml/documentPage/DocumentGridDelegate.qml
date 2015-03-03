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

AbstractButton {
    id: root
    property bool selected: false
    property bool selectionMode: false

    function formattedDateTime() {
        var date = new Date(model.date)
        var diff = model.dateDiff

        if (diff < 1)
            return i18n.tr("Today, ") + Qt.formatTime(date, Qt.locale().timeFormat(Locale.ShortFormat))

        if (diff < 2)
            return i18n.tr("Yesterday, ") + Qt.formatDateTime(date, Qt.locale().timeFormat(Locale.ShortFormat))

        if (diff < 7)
            // TRANSLATORS: this is a datetime formatting string,
            // see http://qt-project.org/doc/qt-5/qml-qtqml-date.html#details for valid expressions.
            return Qt.formatDateTime(date, i18n.tr("dddd, hh:mm"))

        // TRANSLATORS: this is a datetime formatting string,
        // see http://qt-project.org/doc/qt-5/qml-qtqml-date.html#details for valid expressions.
        return Qt.formatDateTime(date, i18n.tr("dd-MM-yyyy hh:mm"))
    }

    Rectangle {
        anchors { fill: parent; margins: units.gu(0.5) }

        color: Qt.lighter(UbuntuColors.lightGrey)
        clip: true

        Loader {
            id: selectionIcon

            anchors {
                right: parent.right
                top: parent.top
            }

            z: 10

            width: (status === Loader.Ready) ? item.implicitWidth : 0
            visible: (status === Loader.Ready) && (item.width === item.implicitWidth)
            Behavior on opacity {
                NumberAnimation {
                    duration: UbuntuAnimation.SnapDuration
                }
            }
        }

        // Document mimetype icon
        Icon {
            anchors.centerIn: parent
            anchors.verticalCenterOffset: - units.gu(2)

            width: units.gu(8); height: width

            // At the moment the suru icon theme doesn't have much icons.
            // Just some note for the future:
            // TODO: Add icons for Office/ODF documents
            // TODO: Whenever there will be icons for source code files, add them.
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

        // Cover
       /* Image {
            anchors.fill: parent

            source: {
                if (model.cover !== "" && typeof model.cover !== "undefined")
                    return model.cover

                if (model.mimetype.toString().indexOf("image") !== -1)
                    return model.path

                return ""
            }

            sourceSize.width: width
            fillMode: Image.PreserveAspectCrop
        }*/

        // Document info overlay
        Rectangle {
            id: overlay

            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }

            height: units.gu(6)

            color: UbuntuColors.darkGrey
            opacity: 0.75
        }

        // Document info
        Column {
            anchors { fill: overlay; margins: units.gu(0.5) }

            Label {
                text: model.name
                color: "white"

                elide: Text.ElideRight
                font.weight: Font.DemiBold
                fontSize: "small"

                anchors { left: parent.left; right: parent.right }
            }

            RowLayout {
                anchors { left: parent.left; right: parent.right }

                Label {
                    text: formattedDateTime()
                    color: "white"                  
                    fontSize: "small"

                    Layout.fillWidth: true
                }

                Label {
                    text: Utils.printSize(model.size)
                    color: "white"
                    fontSize: "small"
                }
            }
        }   // Document info end

        states: [
            State {
                name: "select"
                when: selectionMode || selected
                PropertyChanges {
                    target: selectionIcon
                    source: Qt.resolvedUrl("../upstreamComponents/ListItemWithActionsCheckBox.qml")
                    anchors.margins: units.gu(1)
                }
            }
        ]
    }
}
