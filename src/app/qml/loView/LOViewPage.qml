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
import Ubuntu.Layouts 1.0
import DocumentViewer.LibreOffice 1.0 as LibreOffice

import "../common"
import "../common/utils.js" as Utils
import "KeybHelper.js" as KeybHelper

PageWithBottomEdge {
    id: loPage
    title: {
        var fileName = Utils.getNameOfFile(file.path);

        if (!loPageContentLoader.item)
            return fileName

        switch(loPageContentLoader.item.loDocument.documentType) {
        case 0:
            return fileName + "-" + i18n.tr("LibreOffice text document")
        case 1:
            return fileName + "-" + i18n.tr("LibreOffice spread sheet")
        case 2:
            return fileName + "-" + i18n.tr("LibreOffice presentation")
        case 3:
            return fileName + "-" + i18n.tr("LibreOffice Draw document")
        case 4:
            return fileName + "-" + i18n.tr("Unknown LibreOffice document")
        default:
            return fileName + "-" + i18n.tr("Unknown type document")
        }
    }
    flickable: null

    readonly property bool wideWindow: width > units.gu(120)

    bottomEdgeTitle: i18n.tr("Slides")
    bottomEdgeEnabled: {
        if (!loPageContentLoader.loaded)
            return false

        // else
        return loPageContentLoader.item.loDocument.documentType == LibreOffice.Document.PresentationDocument && !wideWindow
    }

    Loader {
        id: loPageContentLoader

        asynchronous: true
        anchors.fill: parent
        sourceComponent: loPageContentComponent

        onLoaded: {
            if (loaded) {
                // FIXME: At the moment don't hide header if the document is a presentation
                var isPresentation = (item.loDocument.documentType === LibreOffice.Document.PresentationDocument)
                loPage.flickable = isPresentation ? null : item.loView
                loPage.bottomEdgePageComponent = item.bottomEdgePartsPage
            } else loPage.flickable = null
        }
    }

    ActivityIndicator {
        running: loPageContentLoader.status != Loader.Ready
        visible: running
        anchors.centerIn: parent
    }

    Component {
        id: loPageContentComponent

        FocusScope {
            id: loPageContent
            anchors.fill: parent

            property alias loDocument: loView.document
            property alias loView: loView
            property alias bottomEdgePartsPage: bottomEdgePartsPage

            Layouts {
                id: layouts
                anchors.fill: parent

                layouts: [
                    ConditionalLayout {
                        when: wideWindow
                        name: "wideWindowLayout"

                        Item {
                            anchors.fill: parent

                            ResizeableSidebar {
                                id: leftSidebar
                                anchors.left: parent.left
                                anchors.bottom: parent.bottom
                                visible: loDocument.documentType == LO.Document.PresentationDocument

                                PartsView {
                                    id: partsView
                                    anchors.fill: parent
                                    model: loView.partsModel
                                }
                            }

                            Item {
                                anchors {
                                    left: leftSidebar.right
                                    right: parent.right
                                    top: parent.top
                                    bottom: parent.bottom
                                }

                                ItemLayout {
                                    item: "pinchy"
                                    anchors {
                                        top: parent.top
                                        bottom: bottomBarLayoutItem.top
                                        left: parent.left
                                        right: parent.right
                                    }
                                }

                                Item {
                                    id: bottomBarLayoutItem
                                    visible: loDocument.documentType == LibreOffice.Document.PresentationDocument
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
                    }
                ]

                PinchArea {
                    id: pinchy
                    Layouts.item: "pinchy"

                    anchors {
                        left: parent.left
                        right: parent.right
                        top: parent.top
                        bottom: bottomBar.top
                    }

                    // Limits for pinch-to-zoom
                    // FIXME: We should get these limits from the C++ LibreOffice Viewer class
                    property real minimumZoom: 0.5
                    property real maximumZoom: 4.0

                    LibreOffice.Viewer {
                        id: loView
                        objectName: "loView"
                        anchors.fill: parent

                        clip: true
                        documentPath: file.path

                        Behavior on zoomFactor {
                            UbuntuNumberAnimation { duration: UbuntuAnimation.FastDuration }
                        }

                        Component.onCompleted: {
                            // WORKAROUND: Fix for wrong grid unit size
                            flickDeceleration = 1500 * units.gridUnit / 8
                            maximumFlickVelocity = 2500 * units.gridUnit / 8
                        }

                        MouseArea {
                            id: mouseArea
                            anchors.fill: parent

                            onDoubleClicked: {
                                // Limit zoom by double-click to 3.0x factor
                                if (loView.zoomFactor < pinchy.maximumZoom) {
                                    var scaleRatio = 3.0 / loView.zoomFactor

                                    loView.contentX += mouse.x * (scaleRatio - 1)
                                    loView.contentY += mouse.y * (scaleRatio - 1)

                                    loView.zoomFactor = 3.0
                                } else loView.adjustZoomToWidth()
                            }

                            /* Keyboard events */
                            focus: true
                            Keys.onPressed: KeybHelper.parseEvent(event)
                            Component.onCompleted: loPageContent.forceActiveFocus()
                        }

                        Scrollbar { flickableItem: loView; parent: loView.parent }
                        Scrollbar { flickableItem: loView; parent: loView.parent; align: Qt.AlignBottom }
                    }
                }

                // TODO: When we'll have to merge this with the zooming branch, replace this
                // and use a single bottom panel
                SlideControllerPanel {
                    id: bottomBar
                    Layouts.item: "bottomBar"
                    visible: loDocument.documentType == LibreOffice.Document.PresentationDocument
                    height: visible ? units.gu(5) : 0
                    anchors {
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom
                    }
                }
            }

            Component {
                id: bottomEdgePartsPage
                Page {
                    title: i18n.tr("Slides")
                    head.backAction: Action {
                        text: i18n.tr("Back")
                        iconName: "down"
                        onTriggered: pageStack.pop()
                    }

                    flickable: null

                    PartsView {
                        property bool belongsToNestedPage: true
                        anchors.fill: parent
                        model: loView.partsModel
                    }
                }
            }
        }
    }

    // *** HEADER ***
    state: "default"
    states: [
        LOViewDefaultHeader {
            name: "default"
            targetPage: loPage
        }
    ]
}
