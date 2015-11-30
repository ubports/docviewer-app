/*
 * Copyright (C) 2015 Stefano Verzegnassi
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.4
import Ubuntu.Components 1.3

Column {
    spacing: units.gu(4)
    anchors {
        verticalCenter: parent.verticalCenter
        left: parent.left; right: parent.right
    }

    ActivityIndicator {
        running: visible
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Image {
        source: Qt.resolvedUrl("graphics/libreoffice.png")
        fillMode: Image.PreserveAspectFit
        horizontalAlignment: Image.AlignHCenter

        height: units.gu(8)
        anchors { left: parent.left; right: parent.right }
    }

    Label {
        // TRANSLATORS: 'LibreOfficeKit' is the name of the library used by
        // Document Viewer for rendering LibreOffice/MS-Office documents.
        // Ref. https://docs.libreoffice.org/libreofficekit.html
        text: i18n.tr("Powered by LibreOfficeKit")
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
