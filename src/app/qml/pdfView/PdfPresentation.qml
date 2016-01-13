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
    anchors.fill: parent
    title: DocumentViewer.getFileBaseNameFromPath(poppler.path)
    focus: true

    head.locked: true
    head.visible: false
    clip: !head.visible

    head.contents: Item {
        anchors.fill: parent
        anchors.left: pdfPage.left
        Rectangle {
            anchors.fill: parent
            anchors.leftMargin: -units.gu(5)
            color: "white"
            opacity: 0.8
        }
        Label {
            width: parent.width
            anchors.verticalCenter: parent.verticalCenter
            elide: Text.ElideMiddle

            font.weight: Font.DemiBold
            fontSize: "large"
            text: pdfPage.title
            opacity: 1
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
            width: pdfPage.width
            height: pdfPage.height
            Component.onDestruction: window.releaseResources()
        }
        Component.onCompleted: pdfPage.forceActiveFocus()

        MouseArea {
            anchors.fill: parent
            onDoubleClicked: pdfPage.head.visible = !pdfPage.head.visible
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

