/*
 * Copyright (C) 2013-2014 Canonical, Ltd.
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
import Ubuntu.Components 1.2
import Ubuntu.Components.ListItems 1.0 as ListItems

import "utils.js" as Utils

// FIXME: Autopilot tests

Page {
    id: detailsPage
    objectName: "detailsPage"
    title: i18n.tr("Details")

    Column {
        anchors { left: parent.left; right: parent.right }

        ListItem {
            Column {
                anchors {
                    left: parent.left; right: parent.right
                    margins: units.gu(2)
                    verticalCenter: parent.verticalCenter
                }

                Label {
                    text: i18n.tr("Location")
                    color: UbuntuColors.midAubergine
                }
                Label {
                    text: file.path
                    fontSize: "small"
                }
            }
        }

        ListItem {
            Column {
                anchors {
                    left: parent.left; right: parent.right
                    margins: units.gu(2)
                    verticalCenter: parent.verticalCenter
                }

                Label {
                    text: i18n.tr("Size")
                    color: UbuntuColors.midAubergine
                }
                Label {
                    text: Utils.printSize(i18n, file.info.size)
                    fontSize: "small"
                }
            }
        }

        ListItem {
            Column {
                anchors {
                    left: parent.left; right: parent.right
                    margins: units.gu(2)
                    verticalCenter: parent.verticalCenter
                }

                Label {
                    text: i18n.tr("Created")
                    color: UbuntuColors.midAubergine
                }
                Label {
                    text: file.info.creationTime.toLocaleString(Qt.locale())
                    fontSize: "small"
                }
            }
        }

        ListItem {
            Column {
                anchors {
                    left: parent.left; right: parent.right
                    margins: units.gu(2)
                    verticalCenter: parent.verticalCenter
                }

                Label {
                    text: i18n.tr("Last modified")
                    color: UbuntuColors.midAubergine
                }
                Label {
                    text: file.info.lastModified.toLocaleString(Qt.locale())
                    fontSize: "small"
                }
            }
        }

        ListItem {
            // Used by Autopilot tests
            objectName: "mimetypeItem"
            property alias text: mimetypeText.text
            property alias subText: mimetypeSubText.text

            Column {
                anchors {
                    left: parent.left; right: parent.right
                    margins: units.gu(2)
                    verticalCenter: parent.verticalCenter
                }

                Label {
                    id: mimetypeText
                    text: i18n.tr("MIME type")
                    color: UbuntuColors.midAubergine
                }
                Label {
                    id: mimetypeSubText
                    text: file.mimetype.name
                    fontSize: "small"
                }
            }
        }
    }
}
