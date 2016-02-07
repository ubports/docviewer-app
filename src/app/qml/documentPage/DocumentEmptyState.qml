/*
  Copyright (C) 2015, 2016 Stefano Verzegnassi

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

import "../common"

Item {
    anchors.fill: parent

    EmptyState {
        anchors.centerIn: parent
        title: i18n.tr("No documents found")
        subTitle: i18n.tr("Connect your device to any computer and simply drag files to the Documents folder or insert removable media containing documents.")

        Item {
            height: parent.height
            width: imageEmptyDownload.width + units.gu(2)

            Image {
                id: imageEmptyDownload
                anchors.centerIn: parent
                antialiasing: true
                fillMode: Image.PreserveAspectFit
                height: units.gu(10)
                smooth: true
                source: Qt.resolvedUrl("graphics/document_download_icon.png")
            }
        }

        Item {
            height: parent.height
            width: units.gu(7)

            Image {
                anchors.centerIn: parent
                antialiasing: true
                fillMode: Image.PreserveAspectFit
                height: units.gu(6)
                smooth: true
                source: Qt.resolvedUrl("graphics/div.png")
            }
        }

        Image {
            anchors.verticalCenter: parent.verticalCenter
            antialiasing: true
            fillMode: Image.PreserveAspectFit
            height: units.gu(7)
            smooth: true
            source: Qt.resolvedUrl("graphics/sd_phone_icon.png")
        }
    }
}
