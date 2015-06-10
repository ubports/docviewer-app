/*
 * Copyright (C) 2014-2015 Canonical, Ltd.
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
import Ubuntu.Components.Popups 1.0

PageHeadState {
    id: rootItem

    property Page targetPage
    head: targetPage.head

    actions: [
        Action {
            text: i18n.tr("Search...")
            iconName: "search"
            onTriggered: targetPage.searchMode = true

            visible: folderModel.count !== 0
        },

        Action {
            text: i18n.tr("Filters settings...")
            iconSource: Qt.resolvedUrl("../../graphics/settings_alt.svg")
            onTriggered: PopupUtils.open(Qt.resolvedUrl("SortSettingsDialog.qml"))

            visible: folderModel.count !== 0
        },

        Action {
            id: switchView
            text: targetPage.useGridView ? i18n.tr("Switch to single column list") : i18n.tr("Switch to grid")
            iconName: targetPage.useGridView ? "view-list-symbolic" : "view-grid-symbolic"
            onTriggered: targetPage.useGridView = !targetPage.useGridView

            visible: folderModel.count !== 0
        }
    ]
}
