/*
 * Copyright (C) 2013-2015 Canonical, Ltd.
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
import DocumentViewer 1.0
import DocumentViewer.PDF 1.0 as PDF

Page {
    id: pdfPage
    property var poppler
    property bool isPresentationMode: true
    anchors.fill: parent
    title: DocumentViewer.getFileBaseNameFromPath(poppler.path)
    focus: true

    header: PageHeader {
        visible: false
        leadingActionBar.actions: [
            Action {
                iconName: "back"
                text: "Back"
                onTriggered: pageStack.pop()
            }
        ]
        contents: ListItemLayout {
            anchors.centerIn: parent
            title {
                font.weight: Font.DemiBold
                textSize: Label.Large
                text: pdfPage.title
                color: pdfPage.header.__styleInstance.foregroundColor
            }
            subtitle {
                textSize: Label.Medium
                // TRANSLATORS: the first argument (%1) refers to the page currently shown on the screen,
                // while the second one (%2) refers to the total pages count.
                text: i18n.tr("Page %1 of %2").arg(pdfView.currentIndex + 1).arg(pdfView.count)
                color: pdfPage.header.__styleInstance.foregroundColor
            }
        }

        StyleHints {
            backgroundColor: "#BF000000"
            foregroundColor: "white"
        }
    }

    ListView {
        id: pdfView
        anchors.fill: parent
        focus: true

        orientation: Qt.Horizontal;
        snapMode: ListView.SnapOneItem
        highlightRangeMode: ListView.StrictlyEnforceRange
        highlightFollowsCurrentItem: true
        highlightMoveDuration: 0
        boundsBehavior: Flickable.StopAtBounds
        cacheBuffer: width

        model: poppler
        delegate: PdfViewDelegate {
            presentationMode: true
            width: pdfPage.width
            height: pdfPage.height
            color: "black"
            Component.onDestruction: window.releaseResources()
        }
        Component.onCompleted: pdfPage.forceActiveFocus()

        MouseArea {
            anchors.fill: parent
            onDoubleClicked: pdfPage.header.visible = !pdfPage.header.visible

            // Hide mouse curson when there's no on-going mouse event
            hoverEnabled: true
            // Flickable can steal mouse handling from MouseArea,
            // so check for any on-going dragging too
            cursorShape: (showCursorTimer.running || pdfView.dragging) ? Qt.ArrowCursor : Qt.BlankCursor
            onPositionChanged: {
                showCursorTimer.restart()
            }
            Timer {
                id: showCursorTimer
                interval: 500
            }
        }
    }

    Keys.onPressed: {
        if (event.key == Qt.Key_Escape) { pageStack.pop(); return; }
        if (event.key == Qt.Key_Home) { pdfView.positionViewAtBeginning(); return; }
        if (event.key == Qt.Key_End) { pdfView.positionViewAtEnd(); return; }

        if (event.key == Qt.Key_Right || event.key == Qt.Key_PageDown) {
            pdfView.incrementCurrentIndex();
            return;
        }
        if (event.key == Qt.Key_Left || event.key == Qt.Key_PageUp) {
            pdfView.decrementCurrentIndex();
            return;
        }
    }
}

