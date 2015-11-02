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

Page {
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
                                visible: loDocument.documentType == LibreOffice.Document.PresentationDocument

                                PartsView {
                                    id: partsView
                                    anchors.fill: parent
                                    model: loView.partsModel
                                }
                            }

                            ItemLayout {
                                item: "pinchy"
                                anchors {
                                    left: leftSidebar.right
                                    right: parent.right
                                    top: parent.top
                                    bottom: parent.bottom
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

                PartsView {
                    id: bottomBar
                    anchors {
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom
                    }
                    height: visible ? units.gu(12) : 0
                    visible: loDocument.documentType == LibreOffice.Document.PresentationDocument

                    model: loView.partsModel
                    orientation: ListView.Horizontal

                    HorizontalDivider {
                        anchors {
                            left: parent.left
                            right: parent.right
                            top: parent.top
                        }
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
