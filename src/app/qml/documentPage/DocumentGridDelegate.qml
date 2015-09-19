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

import "../common/utils.js" as Utils

TileBase {
    id: root

    function formattedDateTime() {
        var date = new Date(model.date)
        var diff = model.dateDiff

        if (diff < 1)
            // TRANSLATORS: %1 refers to a time formatted as Locale.ShortFormat (e.g. hh:mm). It depends on system settings.
            // http://qt-project.org/doc/qt-4.8/qlocale.html#FormatType-enum
            return i18n.tr("Today, %1").arg(Qt.formatTime(date, Qt.locale().timeFormat(Locale.ShortFormat)))

        if (diff < 2)
            // TRANSLATORS: %1 refers to a time formatted as Locale.ShortFormat (e.g. hh:mm). It depends on system settings.
            // http://qt-project.org/doc/qt-4.8/qlocale.html#FormatType-enum
            return i18n.tr("Yesterday, %1").arg(Qt.formatTime(date, Qt.locale().timeFormat(Locale.ShortFormat)))

        if (diff < 7)
            // TRANSLATORS: this is a datetime formatting string,
            // see http://qt-project.org/doc/qt-5/qml-qtqml-date.html#details for valid expressions.
            return Qt.formatDateTime(date, i18n.tr("dddd, hh:mm"))

        // TRANSLATORS: this is a datetime formatting string,
        // see http://qt-project.org/doc/qt-5/qml-qtqml-date.html#details for valid expressions.
        return Qt.formatDateTime(date, i18n.tr("dd-MM-yyyy hh:mm"))
    }

    title: model.name
    text: formattedDateTime()
    subText: Utils.printSize(i18n, model.size)

    leadingActions: documentDelegateActions.leadingActions
    trailingActions: documentDelegateActions.trailingActions

    Icon {
        id: extStorageIcon

        width: units.gu(4)
        height: units.gu(4)
        anchors {
            left: parent.left
            top: parent.top
            margins: units.gu(0.5)
        }

        visible: model.isFromExternalStorage
        name: "sdcard-symbolic"
    }

    // Document mimetype icon
    Icon {
        anchors.centerIn: parent
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

    DocumentDelegateActions { id: documentDelegateActions }
}
