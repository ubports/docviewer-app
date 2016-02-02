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

Rectangle {
    id: pdfPage

    property int index: model.index
    property bool _previewFetched: false
    property bool presentationMode: false

    property alias status: pageImg.status

    width: parent.width
    height: width * (model.height / model.width)
    color: "#E6E6E6"

    // Preview page rendering. Used as placeholder while zooming the page.
    // We generate the low resolution preview from the texture of the PDF page,
    // so that we can keep page rendering as fast as possible.
    ShaderEffectSource {
        id: previewImg
        anchors.fill: parent

        // We cannot change its opacity or visibility, otherwise the texture will be refreshed,
        // even if live is false.
        live: false
        textureSize: Qt.size(256, 256 * (model.height / model.width))
    }

    Image {
        id: pageImg
        anchors.fill: parent

        cache: false
        source: "image://poppler" + (index % poppler.providersNumber) + "/page/" + index;
        sourceSize.width: pdfPage.width
        fillMode: Image.PreserveAspectFit

        onStatusChanged: {
            if (presentationMode)
                return;

            // This is supposed to run the first time PdfViewDelegate gets the page rendering.
            if (!_previewFetched) {
                if (status == Image.Ready) {
                    previewImg.sourceItem = pageImg
                    // Re-assign sourceItem property, so the texture is not updated when Image status changes.
                    previewImg.sourceItem = pdfPage
                }
            }
        }

        // Request a new page rendering. The order, which pages are requested with, depends on the distance from the currentPage
        Timer {
            id: _zoomTimer
            interval: {
                var diff = Math.abs(pdfView.currentPageIndex - model.index)
                var prov = poppler.providersNumber * 0.5

                if (diff < prov)
                    return 0
                else
                    return (diff - prov) * 10
            }

            onTriggered: {
                pageImg.sourceSize.width = pdfPage.width;
            }
        }
    }

    // Page rendering depends on the width of PdfViewDelegate.
    // Because of this, we have multiple callings to ImageProvider while zooming.
    // Just avoid it.
    Connections {
        target: !presentationMode ? pinchy : null

        onPinchStarted: _zoomTimer.stop();
        onPinchUpdated: {
            // This ensures that page image is not reloaded when the maximumScale or minimumScale has already been reached.
            if ( !(_zoomHelper.scale >= 2.5 && pinch.scale > 1.0) && !(_zoomHelper.scale <= 1.0 && pinch.scale < 1.0) )
                pageImg.sourceSize.width = 0;
        }
        onPinchFinished: _zoomTimer.restart();
    }
}
