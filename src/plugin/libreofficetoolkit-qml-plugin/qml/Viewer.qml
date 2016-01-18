/*
 * Copyright (C) 2015 Canonical, Ltd.
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
import DocumentViewer.LibreOffice 1.0 as LibreOffice

Flickable {
    id: rootFlickable

    property alias document:    view.document
    property alias zoomFactor:  view.zoomFactor
    property alias cacheBuffer: view.cacheBuffer
    property alias partsModel:  view.partsModel
    property alias zoomMode:    view.zoomMode
    property alias error:       view.error

    property string documentPath: ""

    function adjustZoomToWidth()
    {
        var oldZoom = view.zoomFactor
        view.adjustZoomToWidth()

        var zoomScale = view.zoomFactor / oldZoom
        rootFlickable.contentX *= zoomScale
        rootFlickable.contentY *= zoomScale
    }

    function setZoom(newValue)
    {
        var zoomScale = newValue / view.zoomFactor;
        view.zoomFactor = newValue;

        rootFlickable.contentX *= zoomScale;
        rootFlickable.contentY *= zoomScale;
    }

    function moveView(axis, diff)
    {
        if (axis == "vertical") {
            var maxContentY = Math.max(0, rootFlickable.contentHeight - rootFlickable.height)
            rootFlickable.contentY = Math.max(0, Math.min(rootFlickable.contentY + diff, maxContentY ))
        } else {
            var maxContentX = Math.max(0, rootFlickable.contentWidth - rootFlickable.width)
            rootFlickable.contentX = Math.max(0, Math.min(rootFlickable.contentX + diff, maxContentX ))
        }
    }

    function goNextPart()
    {
        document.currentPart = Math.min(document.currentPart + 1, document.partsCount - 1)
    }

    function goPreviousPart()
    {
        document.currentPart = Math.max(0, document.currentPart - 1)
    }

    function goFirstPart()
    {
        document.currentPart = 0
    }

    function goLastPart()
    {
        document.currentPart = document.partsCount - 1
    }

    onDocumentPathChanged: {
        if (documentPath)
            view.initializeDocument(documentPath)
    }

    // zoomFactor is not used here to set contentSize, since it's all managed
    // internally, in the LibreOffice.View component.
    contentHeight: view.height
    contentWidth: view.width

    topMargin: internal.isSpreadSheet ? 0 : Math.max((rootFlickable.height - view.height) * 0.5, 0)
    leftMargin: internal.isSpreadSheet ? 0 : Math.max((rootFlickable.width - view.width) * 0.5, 0)

    boundsBehavior: Flickable.StopAtBounds

    Component.onCompleted: adjustZoomToWidth()

    LibreOffice.View {
        id: view
        parentFlickable: rootFlickable
    }

    Connections {
        target: view.document

        onCurrentPartChanged: {
            // Position view at top-left corner
            rootFlickable.contentX = 0
            rootFlickable.contentY = 0
        }
    }

    QtObject {
        id: internal

        property bool isSpreadSheet: document.documentType == LibreOffice.Document.SpreadsheetDocument
    }
}
