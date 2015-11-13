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

import QtQuick 2.4
import Ubuntu.Components 1.3
import QtQuick.Layouts 1.1
import Ubuntu.Components.Popups 1.3
import DocumentViewer.LibreOffice 1.0 as LibreOffice
import DocumentViewer 1.0

PageHeadState {
    id: rootItem

    property Page targetPage
    head: targetPage.head

    contents: RowLayout {
        anchors.fill: parent
        anchors.rightMargin: units.gu(2)
        spacing: units.gu(1)

        Column {
            id: layout
            Layout.fillWidth: true

            Label {
            anchors { left: parent.left; right: parent.right }
            elide: Text.ElideMiddle
            font.weight: Font.DemiBold
            text: targetPage.title
        }
        Label {
            anchors { left: parent.left; right: parent.right }
            elide: Text.ElideMiddle
            textSize: Label.Small
            text: {
                if (!loPageContentLoader.item)
                    return i18n.tr("Loading...")

                switch(loPageContentLoader.item.loDocument.documentType) {
                case 0:
                    return i18n.tr("LibreOffice text document")
                case 1:
                    return i18n.tr("LibreOffice spread sheet")
                case 2:
                    return i18n.tr("LibreOffice presentation")
                case 3:
                    return i18n.tr("LibreOffice Draw document")
                case 4:
                    return i18n.tr("Unknown LibreOffice document")
                default:
                    return i18n.tr("Unknown type document")
                    }
                }
            }
        }

        ZoomSelector {
            Layout.preferredWidth: units.gu(12)
            Layout.preferredHeight: units.gu(4)

            visible: {
                if (!loPageContentLoader.item)
                    return false

                return DocumentViewer.desktopMode || targetPage.width > units.gu(80)
            }
        }
    }

    actions: [
       /* Action {
            iconName: "Search"
            text: i18n.tr("Search")
            enabled: false
        },*/

        Action {
            // FIXME: Autopilot test broken... seems not to detect we're now using an ActionBar since the switch to UITK 1.3
            objectName: "gotopage"
            iconName: "browser-tabs"
            text: i18n.tr("Go to position...")
            onTriggered: PopupUtils.open(Qt.resolvedUrl("LOViewGotoDialog.qml"), targetPage)
            visible: loPageContentLoader.item.loDocument.documentType == LibreOffice.Document.TextDocument
        },

        Action {
            iconName: "night-mode"
            text: mainView.nightModeEnabled ? i18n.tr("Disable night mode") : i18n.tr("Enable night mode")
            onTriggered: mainView.nightModeEnabled = !mainView.nightModeEnabled
        },

        Action {
            objectName: "detailsAction"
            text: i18n.tr("Details")
            iconName: "info"
            onTriggered: pageStack.push(Qt.resolvedUrl("../common/DetailsPage.qml"))
        }
    ]
}
