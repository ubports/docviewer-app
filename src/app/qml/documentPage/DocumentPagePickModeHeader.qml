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

PageHeadState {
    id: rootItem

    property Page targetPage
    head: targetPage.head

    backAction: Action {
        text: i18n.tr("Cancel")
        objectName: "cancelButton"
        iconName: "close"
        onTriggered: DOC_VIEWER.contentPickingCanceled()
    }

    actions: [
        Action {
            text: targetPage.useGridView ? i18n.tr("Switch to single column list") : i18n.tr("Switch to grid")
            iconName: targetPage.useGridView ? "view-list-symbolic" : "view-grid-symbolic"
            onTriggered: targetPage.useGridView = !targetPage.useGridView

            visible: folderModel.count !== 0
        },

        Action {
            text: i18n.tr("Pick")
            objectName: "pickButton"
            enabled: viewLoader.item.selectedItems.count > 0
            iconName: "ok"
            onTriggered: {
                if (!enabled)
                    return;

                var urlList = []
                var items = documentPage.view.item.selectedItems;

                for (var i=0; i < items.count; i++) {
                    urlList.push(items.get(i).model.path);
                }

                DOC_VIEWER.returnPickedContent(urlList);
            }
        }
    ]
}

