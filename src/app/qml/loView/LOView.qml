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

import QtQuick 2.3
import Ubuntu.Components 1.1
import DocumentViewer.LibreOffice 1.0 as LO

import "../common/utils.js" as Utils
import "../upstreamComponents"

Page {
    id: loPage
    title: Utils.getNameOfFile(file.path);

    // Disable header auto-hide.
    // TODO: Show/hide header if a user taps the page
    flickable: null

    // TRANSLATORS: the first argument (%1) refers to the page currently shown on the screen,
    // while the second one (%2) refers to the total pages count.
    property string currentPage: i18n.tr("Page %1 of %2").arg(loView.currentPageIndex + 1).arg(loView.count)

    // Reset night mode shader settings when closing the page
    // Component.onDestruction: mainView.nightModeEnabled = false

    LO.Viewer {
        id: loView
        objectName: "loView"
        anchors.fill: parent

        clip: true
        documentPath: file.path

        Component.onCompleted: {
            // WORKAROUND: Fix for wrong grid unit size
            flickDeceleration = 1500 * units.gridUnit / 8
            maximumFlickVelocity = 2500 * units.gridUnit / 8
        }
    }

    Scrollbar { flickableItem: loView }
    Scrollbar { flickableItem: loView; align: Qt.AlignBottom }

//    LO.Document {
//        id: loDocument

//        property bool isLoading: true
//        path: file.path
//    }

    // *** HEADER ***
    state: "default"
    states: LOViewDefaultHeader {
        name: "default"
        targetPage: loPage
    }
}
