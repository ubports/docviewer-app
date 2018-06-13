/*
 * Copyright (C) 2014-2016 Canonical, Ltd.
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
import Ubuntu.Components.Popups 1.3
import DocumentViewer.LibreOffice 1.0 as LibreOffice
import DocumentViewer 1.0

PageHeader {
    id: defaultHeader

    property var targetPage

    contents: ListItemLayout {
        anchors.centerIn: parent

        title {
            elide: Text.ElideMiddle
            font.weight: Font.DemiBold
            text: defaultHeader.title
        }

        subtitle {
            textSize: Label.Small
            text: {
                if (!targetPage.contentItem)
                    return i18n.tr("Loading...")

                switch(targetPage.contentItem.loDocument.documentType) {
                case LibreOffice.Document.TextDocument:
                    return i18n.tr("LibreOffice text document")
                case LibreOffice.Document.SpreadsheetDocument:
                    return i18n.tr("LibreOffice spread sheet")
                case LibreOffice.Document.PresentationDocument:
                    return i18n.tr("LibreOffice presentation")
                case LibreOffice.Document.DrawingDocument:
                    return i18n.tr("LibreOffice Draw document")
                case LibreOffice.Document.OtherDocument:
                    return i18n.tr("Unknown LibreOffice document")
                default:
                    return i18n.tr("Unknown document type")
                }
            }
        }

        ZoomSelector {
            SlotsLayout.position: SlotsLayout.Trailing
            view: targetPage.contentItem.loView
            visible: targetPage.contentItem && (DocumentViewer.desktopMode || mainView.wideWindow)
        }
    }

    trailingActionBar.actions: [
        Action {
            // FIXME: Autopilot test broken... seems not to detect we're now using an ActionBar since the switch to UITK 1.3
            objectName: "gotopage"
            iconName: "browser-tabs"
            text: i18n.tr("Go to position…")
            visible: targetPage.contentItem.loDocument.documentType == LibreOffice.Document.TextDocument

            onTriggered: {
                var popupSettings = {
                    view: targetPage.contentItem.loView
                }

                PopupUtils.open(Qt.resolvedUrl("LOViewGotoDialog.qml"), targetPage, popupSettings)
            }
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
