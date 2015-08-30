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
import QtGraphicalEffects 1.0

import "../common/utils.js" as Utils
import "../upstreamComponents"

ListItemWithActions {
    property QtObject documentDelegateActions: DocumentDelegateActions { }

    function formattedDateTime() {
        var date = new Date(model.date)
        var diff = model.dateDiff

        if (sortSettings.sortMode !== 0) {  // The sort rule is not "by date"
            switch(diff) {
            case 0:     // DocumentsModel.Today
                // TRANSLATORS: %1 refers to a time formatted as Locale.ShortFormat (e.g. hh:mm). It depends on system settings.
                // http://qt-project.org/doc/qt-4.8/qlocale.html#FormatType-enum
                return i18n.tr("Today, %1").arg(Qt.formatTime(date, Qt.locale().timeFormat(Locale.ShortFormat)))

            case 1:     // DocumentsModel.Yesterday
                // TRANSLATORS: %1 refers to a time formatted as Locale.ShortFormat (e.g. hh:mm). It depends on system settings.
                // http://qt-project.org/doc/qt-4.8/qlocale.html#FormatType-enum
                return i18n.tr("Yesterday, %1").arg(Qt.formatTime(date, Qt.locale().timeFormat(Locale.ShortFormat)))

            default:    // DocumentsModel.LastWeek || DocumentsModel.LastMonth || DocumentsModel.Earlier
                // TRANSLATORS: this is a datetime formatting string,
                // see http://qt-project.org/doc/qt-5/qml-qtqml-date.html#details for valid expressions.
                return Qt.formatDateTime(date, i18n.tr("dd-MM-yyyy hh:mm"))
            }
        } else {
            switch(diff) {
            case 0:     // DocumentsModel.Today, or
            case 1:     // DocumentsModel.Yesterday
                return Qt.formatDateTime(date, Qt.locale().timeFormat(Locale.ShortFormat))

            case 2:     // DocumentsModel.LastWeek
                // TRANSLATORS: this is a datetime formatting string,
                // see http://qt-project.org/doc/qt-5/qml-qtqml-date.html#details for valid expressions.
                return Qt.formatDateTime(date, i18n.tr("dddd, hh:mm"))

            default:    // DocumentsModel.LastMonth || DocumentsModel.Earlier
                // TRANSLATORS: this is a datetime formatting string,
                // see http://qt-project.org/doc/qt-5/qml-qtqml-date.html#details for valid expressions.
                return Qt.formatDateTime(date, i18n.tr("dd-MM-yyyy hh:mm"))
            }
        }
    }

    anchors { left: parent.left; right: parent.right }
    height: units.gu(8)

    locked: documentPage.state == "pickMode"

    leftSideAction: documentDelegateActions.leadingActions[0]
    rightSideActions: documentDelegateActions.trailingActions

    contents: RowLayout {
        anchors.fill: parent
        spacing: units.gu(2)

        Icon {
            Layout.preferredWidth: height
            Layout.preferredHeight: units.gu(5)

            // At the moment the suru icon theme doesn't have much icons.
            name: {
                if (model.mimetype.substring(0, 5) === "text/")
                    return "text-x-generic-symbolic"

                if (model.mimetype.substring(0, 5) === "image")
                    return "image-x-generic-symbolic"

                if (model.mimetype === "application/pdf")
                    return "application-pdf-symbolic"

                if (model.mimetype === "application/vnd.oasis.opendocument.text"
                        || model.mimetype === "application/msword"
                        || model.mimetype === "application/vnd.openxmlformats-officedocument.wordprocessingml.document")
                    return "x-office-document-symbolic"

                if (model.mimetype === "application/vnd.oasis.opendocument.spreadsheet"
                        || model.mimetype === "application/vnd.ms-excel"
                        || model.mimetype === "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet")
                    return "x-office-spreadsheet-symbolic"

                if (model.mimetype === "application/vnd.oasis.opendocument.presentation"
                        || model.mimetype === "application/vnd.ms-powerpoint"
                        || model.mimetype === "application/vnd.openxmlformats-officedocument.presentationml.presentation")
                    return "x-office-presentation-symbolic"

                return "package-x-generic-symbolic"
            }

            Rectangle {
                anchors {
                    right: parent.right
                    bottom: parent.bottom
                }

                width: units.gu(2.5)
                height: units.gu(2.5)

                visible: model.isFromExternalStorage
                color: mainView.backgroundColor

                Icon {
                    anchors.fill: parent
                    source: Qt.resolvedUrl("../../graphics/sd-card-symbolic.png")
                }
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
                    text: Utils.printSize(i18n, model.size)
                    fontSize: "small"
                }
            }
        }
    }
}
