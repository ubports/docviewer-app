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
import QtQuick.Layouts 1.1
import Ubuntu.Components.ListItems 1.3 as ListItems

Rectangle {
    id: bottomBar
    color: "transparent"
    height: units.gu(5)

    ListItems.ThinDivider {
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
    }

    Row {
        anchors.centerIn: parent
        //spacing: units.gu(2)

        AbstractButton {
            width: units.gu(4); height: parent.height
            onClicked: loPageContentLoader.item.loView.goFirstPart()

            Icon {
                anchors.centerIn: parent
                width: units.gu(2.5); height: width
                name: "go-first"
            }
        }

        AbstractButton {
            width: units.gu(4); height: parent.height
            onClicked: loPageContentLoader.item.loView.goPreviousPart()

            Icon {
                anchors.centerIn: parent
                width: units.gu(2.5); height: width
                name: "go-previous"
            }
        }

        Label {
            text: i18n.tr("Slide %1 of %2").arg(loPageContentLoader.item.loDocument.currentPart + 1).arg(loPageContentLoader.item.loDocument.partsCount)
        }

        AbstractButton {
            width: units.gu(4); height: parent.height
            onClicked: loPageContentLoader.item.loView.goNextPart()

            Icon {
                anchors.centerIn: parent
                width: units.gu(2.5); height: width
                name: "go-next"
            }
        }

        AbstractButton {
            width: units.gu(4); height: parent.height
            onClicked: loPageContentLoader.item.loView.goLastPart()

            Icon {
                anchors.centerIn: parent
                width: units.gu(2.5); height: width
                name: "go-last"
            }
        }
    }
}
