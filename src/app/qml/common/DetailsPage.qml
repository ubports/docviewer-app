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

import QtQuick 2.3
import Ubuntu.Components 1.1
import Ubuntu.Components.ListItems 1.0 as ListItem

import "utils.js" as Utils

Page {
    id: detailsPage
    objectName: "detailsPage"

    title: i18n.tr("Details")

    Column {
        width: parent.width

        ListItem.Subtitled {
            text: i18n.tr("Location")
            subText: file.path
        }
        ListItem.Subtitled {
            text: i18n.tr("Size")
            subText: Utils.printSize(i18n, file.size)
        }

        ListItem.Subtitled {
            text: i18n.tr("Created")
            subText: file.creationTime.toLocaleString(Qt.locale())
        }

        ListItem.Subtitled {
            text: i18n.tr("Last modified")
            subText: file.lastModified.toLocaleString(Qt.locale())
        }

        ListItem.Subtitled {
            id: mimetypeItem
            objectName: "mimetypeItem"
            text: i18n.tr("MIME type")
            subText: file.mimetype
        }
    }
}
