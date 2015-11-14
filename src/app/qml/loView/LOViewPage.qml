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
import Ubuntu.Components 1.2
import Ubuntu.Layouts 1.0
import DocumentViewer.LibreOffice 1.0 as LibreOffice

import "../upstreamComponents"

import "../common/utils.js" as Utils
import "KeybHelper.js" as KeybHelper

PageWithBottomEdge {
    id: loPage
    title: Utils.getNameOfFile(file.path);
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

            } else {
                loPage.flickable = null
            }
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

                            // TODO: Add a setting to show/hide sidebar when width > units.gu(80)
                            PartsView {
                                id: partsView
                                anchors {
                                    top: parent.top
                                    bottom: bottomBarLayoutItem.top
                                    left: parent.left
                                }

                                model: loView.partsModel
                                visible: loDocument.documentType == LibreOffice.Document.PresentationDocument
                                width: visible ? units.gu(40) : 0
                            }

                            Item {
                                anchors {
                                    left: partsView.right
                                    right: parent.right
                                    top: parent.top
                                    bottom: bottomBarLayoutItem.top
                                }

                                ItemLayout {
                                    item: "loView"
                                    anchors.fill: parent

                                    // Keyboard events
                                    focus: true
                                    Keys.onPressed: KeybHelper.parseEvent(event)
                                    Component.onCompleted: loPageContent.forceActiveFocus()
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
                ]



//                LibreOffice.Viewer {
//                    id: loView
//                    objectName: "loView"
//                    Layouts.item: "loView"

//                    anchors.fill: parent

//                    clip: true
//                    documentPath: file.path

//                    // Keyboard events
//                    focus: true
//                    Keys.onPressed: KeybHelper.parseEvent(event)

//                    Component.onCompleted: {
//                        // WORKAROUND: Fix for wrong grid unit size
//                        flickDeceleration = 1500 * units.gridUnit / 8
//                        maximumFlickVelocity = 2500 * units.gridUnit / 8
//                        loPageContent.forceActiveFocus()
//                    }

//                    Scrollbar { flickableItem: loView; parent: loView.parent }
//                    Scrollbar { flickableItem: loView; parent: loView.parent; align: Qt.AlignBottom }
//                }

                // -------------------------------------------------------------------

                PinchArea {
                    id: pinchArea

                    property real baseScale: 1
                    property var myItem: loView

                    anchors {
                        left: parent.left
                        right: parent.right
                        top: parent.top
                        bottom: bottomBar.top
                    }

                    onPinchUpdated: {
                        pinchUpdatedHandler(pinch)
                    }
                    onPinchFinished: {
                        pinchFinishedHandler()
                    }

                    function pinchUpdatedHandler(pinch) {
                        myItem.transformScale = pinch.scale
                        //myItem.originX = pinch.center.x
                        //myItem.originY = pinch.center.y
                    }

                    // Component.onCompleted: myItem.transformOrigin = Item.TopLeft
                    function pinchFinishedHandler() {
                        var pt = pinchArea.mapFromItem(myItem, -myItem.contentX , -myItem.contentY )
                        console.log("pinchFinishedHandler", -myItem.contentX, -myItem.contentY, Math.round(pt.x), Math.round(pt.y))
                        myItem.contentX = -pt.x
                        myItem.contentY = -pt.y

                        baseScale = myItem.transformScale * baseScale
                        myItem.updateContentSize(baseScale)
                        myItem.transformScale = 1
                    }

                    LibreOffice.Viewer {
                        id: loView
                        objectName: "loView"
                        Layouts.item: "loView"

                        anchors.fill: parent

                        property real transformScale: 1
                        property real originX: 0
                        property real originY: 0
                        scale: transformScale

                        clip: true
                        documentPath: file.path

                        // Keyboard events
                        focus: true
                        Keys.onPressed: KeybHelper.parseEvent(event)

                        Component.onCompleted: {
                            // WORKAROUND: Fix for wrong grid unit size
                            flickDeceleration = 1500 * units.gridUnit / 8
                            maximumFlickVelocity = 2500 * units.gridUnit / 8
                            loPageContent.forceActiveFocus()
                        }

                        function updateContentSize(tgtScale) {
                            zoomFactor = tgtScale
                        }

                        Scrollbar { flickableItem: loView; parent: loView.parent }
                        Scrollbar { flickableItem: loView; parent: loView.parent; align: Qt.AlignBottom }
                    }

                    // ------------------------ Desktop DEBUG

                    MouseArea {
                        id: testMa
                        anchors.fill: parent
                        visible: false //  Qt.platform.os == "linux"
                        z: 9999999
                        acceptedButtons: Qt.RightButton

                        property int touchPointY
                        property int touchPointX
                        onPressed: {
                            console.log("PRESSED")
                            touchPointY = mouse.y
                            touchPointX = mouse.x
                        }
                        onPositionChanged: {
                            var diff = mouse.y - touchPointY
                            var sc = (1 + diff / 200)
                            //pinchUpdatedHandler({"center" : { "x" : touchPointX, "y" : touchPointY }, "scale" : sc })
                            pinchUpdatedHandler({"center" : { "x" : mouse.x, "y" : mouse.y }, "scale" : sc })
                        }
                        onReleased: {
                            pinchFinishedHandler()
                        }
                    }

                    // ------------------------ Desktop DEBUG end

//                    Flickable {
//                        //id: flickable
//                        id: myItem

//                        property real transformScale: 1
//                        property real originX: 0
//                        property real originY: 0

//                        //        transform: [
//                        //            Scale {
//                        //                origin {
//                        //                    x: myItem.originX
//                        //                    y: myItem.originY
//                        //                }
//                        //                xScale: myItem.transformScale
//                        //                yScale: myItem.transformScale
//                        //            }
//                        //        ]
//                        scale: transformScale

//                        function updateContentSize(tgtScale) {
//                            image.width = image.sourceSize.width * tgtScale
//                            image.height = image.sourceSize.height * tgtScale
//                        }

//                        anchors.fill: parent
//                        contentWidth: image.width; contentHeight: image.height

//                        Image { id: image; source: "qrc:/back.jpg" }
//                    }
                }

                // -------------------------------------------------------------------

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
        },

        LOViewZoomHeader {
            name: "zoom"
            targetPage: loPage
        }
    ]
}
