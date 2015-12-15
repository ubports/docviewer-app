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
import DocumentViewer.PDF 1.0 as PDF

import "../common"
import "../common/utils.js" as Utils

Page {
    id: pdfPage
    title: Utils.getNameOfFile(file.path)

    flickable: pdfView

    // TRANSLATORS: the first argument (%1) refers to the page currently shown on the screen,
    // while the second one (%2) refers to the total pages count.
    property string currentPage: i18n.tr("Page %1 of %2").arg(pdfView.currentPageIndex + 1).arg(pdfView.count)

    BottomEdge {
        id: bottomEdge

        hint {
            action: Action {
                // TRANSLATORS: "Contents" refers to the "Table of Contents" of a PDF document.
                text: i18n.tr("Contents")
                iconName: "view-list-symbolic"  // FIXME: Needs ToC icon.
                onTriggered: bottomEdge.commit()
            }
            flickable: pdfPage.flickable
        }

        contentComponent: bottomEdgeContent
        onCommitCompleted: {
            pdfPage.pageStack.push(bottomEdgeContent)
            collapse();
        }
        enabled: poppler.tocModel.count > 0

        Component {
            id: bottomEdgeContent
            PdfContentsPage { width: bottomEdge.width; height: bottomEdge.height }
        }
    }


    // Reset night mode shader settings when closing the page
    // Component.onDestruction: mainView.nightModeEnabled = false

    Rectangle {
        // Since UITK 1.3, the MainView background is white.
        // We need to set a different color, otherwise pages
        // boundaries are not visible.
        anchors.fill: parent
        color: "#f5f5f5"
    }

    PDF.VerticalView {
        id: pdfView
        objectName: "pdfView"

        anchors {
            fill: parent

            // WORKAROUND: If we set 'pdfPage.flickable' property, 'pdfView' is not
            // longer aligned to the bottom of the header, but to the top instead.
            // This is deprecated code though, and it's not worth to spend more time
            // for a proper fix here.
            // (This is likely a bug in the VerticalView class.)
            topMargin: units.gu(6)
        }

        spacing: units.gu(2)

        boundsBehavior: Flickable.StopAtBounds
        flickDeceleration: 1500 * units.gridUnit / 8
        maximumFlickVelocity: 2500 * units.gridUnit / 8

        contentWidth: parent.width * _zoomHelper.scale
        cacheBuffer: height * poppler.providersNumber * _zoomHelper.scale * 0.5
        interactive: !pinchy.pinch.active

        model: poppler
        delegate: PdfViewDelegate {
            Component.onDestruction: window.releaseResources()
        }

        // FIXME: On zooming, keep the same content position.
        PinchArea {
            id: pinchy
            anchors.fill: parent

            pinch {
                target: _zoomHelper
                minimumScale: 1.0
                maximumScale: 2.5
            }

            onPinchFinished: {
                pdfView.returnToBounds();

                // This is a bit expensive, so it's safer to put it here.
                // It won't be called on desktop (where PinchArea is not used),
                // but it's not a problem at the moment (our target is phone).
                window.releaseResources();
            }
        }

        Item { id: _zoomHelper }
    }


    Scrollbar { flickableItem: pdfView }
    Scrollbar { flickableItem: pdfView; align: Qt.AlignBottom }

    PDF.Document {
        id: poppler

        property bool isLoading: true
        path: file.path

        onPagesLoaded: {
            isLoading = false;

            var title = getDocumentInfo("Title")
            if (title !== "")
                pdfPage.title = title;
        }
    }

    // *** HEADER ***
    state: "default"
    states: PdfViewDefaultHeader {
        name: "default"
        targetPage: pdfPage
        activityRunning: pdfView.currentPageItem.status == Image.Loading || poppler.isLoading
    }
}
