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

    property alias zoomMode:    view.zoomMode

    function adjustZoomToWidth()
    {
        view.adjustZoomToWidth();
    }

    // zoomFactor is not used here to set contentSize, since it's all managed
    // internally, in the LibreOffice.View component.
    contentHeight: view.height
    contentWidth: view.width

    boundsBehavior: Flickable.StopAtBounds

    Component.onCompleted: adjustZoomToWidth()

    LibreOffice.View {
        id: view

        parentFlickable: rootFlickable
    }
}
