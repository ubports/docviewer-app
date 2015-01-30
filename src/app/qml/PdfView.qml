/*
 * Copyright (C) 2013-2014 Canonical, Ltd.
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
import com.ubuntu.popplerqmlplugin 1.0 as PDF

import "utils.js" as Utils

Page {
    id: pdfPage
    title: Utils.getNameOfFile(file.path);

    // Disable header auto-hide.
    // TODO: Show/hide header if a user taps the page
    flickable: null

    property string currentPage: i18n.tr("Page %1 of %2").arg(pdfView.currentIndex + 1).arg(pdfView.count)

    PDF.VerticalView {
        id: pdfView
        objectName:"pdfView"

        anchors.fill: parent
        spacing: units.gu(2)

        clip: true
        boundsBehavior: Flickable.StopAtBounds

        cacheBuffer: height * poppler.providersNumber * _zoomHelper.scale

        flickDeceleration: 1500 * units.gridUnit / 8
        maximumFlickVelocity: 2500 * units.gridUnit / 8

        model: poppler
        delegate: PdfViewDelegate {
            onWidthChanged: QQuickView.releaseResources()
            Component.onDestruction: QQuickView.releaseResources()
        }

        contentWidth: parent.width * _zoomHelper.scale
        PinchArea {
            id: pinchy
            anchors.fill: parent

            pinch {
                target: _zoomHelper
                minimumScale: 1.0
                maximumScale: 2.5
            }

            onPinchStarted: pdfView.interactive = false;

            // FIXME: On zooming, keep the same content position.
            // onPinchUpdated: {}

            onPinchFinished: {
                pdfView.interactive = true;
                pdfView.returnToBounds();
            }
        }

        Item { id: _zoomHelper }
    }

    PDF.Document {
        id: poppler

        property bool isLoading: true

        Component.onCompleted: path = file.path
        onPagesLoaded: {
            isLoading = false;

            var title = getDocumentInfo("Title")
            if (title !== "")
                pdfPage.title = title
        }
    }


    // *** HEADER ***
    state: "default"
    states: [
        PdfViewDefaultHeader {
            name: "default"
            targetPage: pdfPage
        }
    ]
}
