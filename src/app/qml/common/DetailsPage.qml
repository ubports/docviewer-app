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

import "utils.js" as Utils

Page {
    id: detailsPage
    objectName: "detailsPage"
    title: i18n.tr("Details")

    Column {
        width: Math.min(units.gu(80), parent.width)
        anchors {
            top: parent.top
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }

        SubtitledListItem {
            text: i18n.tr("Location")
            subText: file.path
        }

        SubtitledListItem {
            text: i18n.tr("Size")
            subText: Utils.printSize(i18n, file.info.size)
        }

        SubtitledListItem {
            text: i18n.tr("Created")
            subText: file.info.creationTime.toLocaleString(Qt.locale())
        }

        SubtitledListItem {
            text: i18n.tr("Last modified")
            subText: file.info.lastModified.toLocaleString(Qt.locale())
        }

        SubtitledListItem {
            // Used by Autopilot tests
            objectName: "mimetypeItem"
            text: i18n.tr("MIME type")
            subText: file.mimetype.name
        }
    }
}
