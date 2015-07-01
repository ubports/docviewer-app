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
        text: i18n.tr("Back")
        iconName: "back"

        onTriggered: {
            // Clear the search
            searchField.text = ""
            targetPage.searchMode = false
        }
    }

    contents: TextField {
        id: searchField
        width: parent.width - units.gu(4)

        primaryItem: Icon {
            height: parent.height - units.gu(2)
            width: height
            name: "search"
        }

        placeholderText: i18n.tr("search in documents...")
        onTextChanged: folderModel.search(text)

        // Disable predictive text
        inputMethodHints: Qt.ImhNoPredictiveText

        // Force active focus when this becomes the current PageHead state and
        // show OSK if appropriate.
        onVisibleChanged: forceActiveFocus()
    }
}
