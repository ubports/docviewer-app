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

import QtQuick 2.4
import Ubuntu.Components 1.3
import QtQuick.Layouts 1.1
import DocumentViewer 1.0

import "../common/utils.js" as Utils

// TODO: Ask for a review of this component to the design team

ListItem {
    id: listDelegate
    height: units.gu(9)
    leadingActions: ListItemActions { actions: documentDelegateActions.leadingActions }
    trailingActions: ListItemActions { actions: documentDelegateActions.trailingActions }

    RowLayout {
        spacing: units.gu(2)
        anchors {
            fill: parent; margins: units.gu(1)
            leftMargin: units.gu(2)
            rightMargin: units.gu(2)
        }

        Icon {
            name: Utils.getIconNameFromMimetype(model.mimetype)
            Layout.preferredWidth: height
            Layout.preferredHeight: units.gu(5)
        }

        Column {
            Layout.fillWidth: true

            RowLayout {
                width: parent.width
                Label {
                    text: model.name
                    //wrapMode: Text.Wrap
                    elide: Text.ElideRight
                    color: UbuntuColors.midAubergine
                    Layout.fillWidth: true
                }
                Label {
                    text: Utils.printSize(i18n, model.size)
                    textSize: Label.Small
                }
            }

            RowLayout {
                width: parent.width
                Label {
                    text: internal.formattedDateTime()
                    textSize: Label.Small

                    Layout.fillWidth: true
                }
                Icon {
                    width: units.gu(2); height: width
                    name: "sdcard-symbolic"
                    visible: model.isFromExternalStorage
                }
            }
        }
    }

    DocumentDelegateActions { id: documentDelegateActions }

    QtObject {
        id: internal

        function formattedDateTime() {
            var date = new Date(model.date)
            var diff = model.dateDiff

            if (sortSettings.sortMode !== 0) {  // Sort is not "by date"
                switch(diff) {
                case DocumentsModel.Today:
                    // TRANSLATORS: %1 refers to a time formatted as Locale.ShortFormat (e.g. hh:mm). It depends on system settings.
                    // http://qt-project.org/doc/qt-4.8/qlocale.html#FormatType-enum
                    return i18n.tr("Today, %1").arg(Qt.formatTime(date, Qt.locale().timeFormat(Locale.ShortFormat)))

                case DocumentsModel.Yesterday:
                    // TRANSLATORS: %1 refers to a time formatted as Locale.ShortFormat (e.g. hh:mm). It depends on system settings.
                    // http://qt-project.org/doc/qt-4.8/qlocale.html#FormatType-enum
                    return i18n.tr("Yesterday, %1").arg(Qt.formatTime(date, Qt.locale().timeFormat(Locale.ShortFormat)))

                case DocumentsModel.LastWeek:
                case DocumentsModel.LastMonth:
                case DocumentsModel.Earlier:
                    // TRANSLATORS: this is a datetime formatting string,
                    // see http://qt-project.org/doc/qt-5/qml-qtqml-date.html#details for valid expressions.
                    return Qt.formatDateTime(date, i18n.tr("yyyy/MM/dd hh:mm"))
                }
            }

            else {  //
                switch(diff) {  // Sort "by date"
                case DocumentsModel.Today:
                case DocumentsModel.Yesterday:
                    return Qt.formatDateTime(date, Qt.locale().timeFormat(Locale.ShortFormat))

                case DocumentsModel.LastWeek:
                    // TRANSLATORS: this is a datetime formatting string,
                    // see http://qt-project.org/doc/qt-5/qml-qtqml-date.html#details for valid expressions.
                    return Qt.formatDateTime(date, i18n.tr("dddd, hh:mm"))

                case DocumentsModel.LastMonth:
                case DocumentsModel.Earlier:
                    // TRANSLATORS: this is a datetime formatting string,
                    // see http://qt-project.org/doc/qt-5/qml-qtqml-date.html#details for valid expressions.
                    return Qt.formatDateTime(date, i18n.tr("yyyy/MM/dd hh:mm"))
                }
            }
        }
    }
}
