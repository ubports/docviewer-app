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
import DocumentViewer 1.0

PageHeadState {
    id: rootItem

    property Page targetPage
    property alias activityRunning: activity.running

    head: targetPage.head

    contents: RowLayout {
        anchors.fill: parent
        spacing: units.gu(1)

        ActivityIndicator { id: activity; visible: running }

        Column {
            id: layout
            Layout.fillWidth: true
            visible: !DocumentViewer.desktopMode

            Label {
                width: parent.width
                //horizontalAlignment: Text.AlignHCenter
                elide: Text.ElideMiddle

                font.weight: Font.DemiBold
                text: targetPage.title
            }
            Label {
                width: parent.width
                //horizontalAlignment: Text.AlignHCenter
                elide: Text.ElideMiddle

                textSize: Label.Small
                text: targetPage.currentPage
            }
        }
    }

    backAction: Action {
        iconName: "back"
        text: (pageStack.depth > 1) ? i18n.tr("Back") : i18n.tr("Close")
        onTriggered: {
            if (pageStack.depth > 1) {
                // Go back to Welcome page
                pageStack.pop();
            } else {
                // File has been imported through Content Hub (or was not chosen through WelcomePage)
                // Close the application and show our source app (e.g. ubuntu-filemanager-app, if used to open a document)
                Qt.quit()
            }
        }
    }

    actions: [
        Action {
            iconName: "search"
            // onTriggered: pageMain.state = "search"
            //Disable it until we provide search in Poppler plugin.
            enabled: false
        },

        Action {
            objectName:"gotopage"
            iconName: "browser-tabs"
            text: i18n.tr("Go to page...")
            onTriggered: PopupUtils.open(Qt.resolvedUrl("PdfViewGotoDialog.qml"), targetPage)
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
