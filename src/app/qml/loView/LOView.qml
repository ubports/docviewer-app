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
import Ubuntu.Layouts 1.0
import DocumentViewer.LibreOffice 1.0 as LO

import "../upstreamComponents"

import "../common/utils.js" as Utils
import "KeybHelper.js" as KeybHelper

PageWithBottomEdge {
    id: loPage
    title: Utils.getNameOfFile(file.path);

    // FIXME: At the moment don't hide header if the document is a presentation
    flickable: loDocument.documentType != LO.Document.PresentationDocument ? loView : null

    readonly property bool wideWindow: width > units.gu(120)
 
    function moveView(axis, diff) {
        if (axis == "vertical") {
            var maxContentY = Math.max(0, loView.contentHeight - loView.height)
            loView.contentY = Math.max(0, Math.min(loView.contentY + diff, maxContentY ))
        } else {
            var maxContentX = Math.max(0, loView.contentWidth - loView.width)
            loView.contentX = Math.max(0, Math.min(loView.contentX + diff, maxContentX ))
        }
    }

    // Reset night mode shader settings when closing the page
    // Component.onDestruction: mainView.nightModeEnabled = false

    // LibreOfficeKit parts are only supported for Presentation type
    bottomEdgeEnabled: loDocument.documentType == LO.Document.PresentationDocument && !wideWindow
    bottomEdgeTitle: i18n.tr("Slides")
    bottomEdgePageComponent: Page {
        title: i18n.tr("Slides")
        head.backAction: Action {
            text: i18n.tr("Back")
            iconName: "down"
            onTriggered: pageStack.pop()
        }

        flickable: null

        PartsView {
            anchors.fill: parent
            model: partsModelLoader.item
        }
    }

    Layouts {
        id: layouts
        anchors.fill: parent

        layouts: [
            ConditionalLayout {
                when: wideWindow
                name: "wideWindowLayout"

                Item {
                    anchors.fill: parent

                    // TODO: Add a setting to show/hide sidebar when width > units.gu(80)
                    PartsView {
                        id: partsView
                        anchors {
                            top: parent.top
                            bottom: bottomBarLayoutItem.top
                            left: parent.left
                        }

                        model: partsModelLoader.item
                        visible: model
                        width: visible ? units.gu(40) : 0
                    }

                    Item {
                        anchors {
                            left: partsView.right
                            right: parent.right
                            top: parent.top
                            bottom: bottomBarLayoutItem.top
                        }
                        ItemLayout { item: "loView"; anchors.fill: parent }
                    }

                    Item {
                        id: bottomBarLayoutItem
                        visible: loDocument.documentType == LO.Document.PresentationDocument
                        height: visible ? units.gu(5) : 0
                        anchors {
                            left: parent.left
                            right: parent.right
                            bottom: parent.bottom
                        }

                        ItemLayout { item: "bottomBar"; anchors.fill: parent }
                    }
                }
            }
        ]

        LO.Viewer {
            id: loView
            objectName: "loView"
            Layouts.item: "loView"
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
                bottom: bottomBar.top
            }
            anchors.fill: parent

            clip: true
            document: loDocument

            Component.onCompleted: {
                // WORKAROUND: Fix for wrong grid unit size
                flickDeceleration = 1500 * units.gridUnit / 8
                maximumFlickVelocity = 2500 * units.gridUnit / 8
            }

            Scrollbar { flickableItem: loView; parent: loView.parent }
            Scrollbar { flickableItem: loView; parent: loView.parent; align: Qt.AlignBottom }
        }

        // TODO: When we'll have to merge this with the zooming branch, replace this
        // and use a single bottom panel
        SlideControllerPanel {
            id: bottomBar
            Layouts.item: "bottomBar"
            visible: loDocument.documentType == LO.Document.PresentationDocument
            height: visible ? units.gu(5) : 0
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
        }
    }

    LO.Document {
        id: loDocument
        property bool isLoading: true
        path: file.path
    }

    // We use a loader here, since not all the document types support LOK:Parts
    // This prevents the thumbnail provider to try a tile rendering of the part.
    Loader {
        id: partsModelLoader
        active: loDocument.documentType == LO.Document.PresentationDocument
        sourceComponent: LO.PartsModel { document: loDocument }
    }

    // *** HEADER ***
    state: "default"
    states: [
        LOViewDefaultHeader {
            name: "default"
            targetPage: loPage
        },

        LOViewZoomHeader {
            name: "zoom"
            targetPage: loPage
        }
    ]
}
