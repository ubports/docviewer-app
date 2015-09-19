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
    flickable: null

    Loader {
        id: contentLoader

        asynchronous: true
        anchors.fill: parent
        sourceComponent: loPageContentComponent
    }

    ActivityIndicator {
        running: contentLoader.status != Loader.Ready
        visible: running
        anchors.centerIn: parent
    }

    Component {
        id: loPageContentComponent

        Item {
            property alias loDocument: loView.document

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

            // *** HEADER ***
            state: "default"
            states: LOViewDefaultHeader {
                name: "default"
                targetPage: loPage
            }
        }
    }
}
