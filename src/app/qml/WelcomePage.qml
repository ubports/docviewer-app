/*
 * Copyright (C) 2015 Canonical, Ltd.
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

import QtQuick 2.0
import Ubuntu.Components 1.1

Page {
    id: welcomePage

    title: i18n.tr("Document Viewer")
    head.actions: [ openAction ]

    EmptyState {
        title: i18n.tr("No opened documents")
        subTitle: i18n.tr("Tap the + icon to open a document")

        iconName: "edit-copy"

        anchors.centerIn: parent
    }

    Action {
        id: openAction
        text: i18n.tr("Open a file...")
        iconName: "add"

        onTriggered: pageStack.push(Qt.resolvedUrl("ContentHubPicker.qml"))
    }
}
