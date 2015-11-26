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
import "../common"
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
                                    item: "pinchArea"
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

                ScalingPinchArea {
                    id: pinchArea
                    objectName: "pinchArea"
                    Layouts.item: "pinchArea"

                    targetFlickable: loView
                    onTotalScaleChanged: targetFlickable.updateContentSize(totalScale)

                    anchors {
                        top: parent.top
                        left: parent.left
                        right: parent.right
                        bottom: bottomBar.top
                    }

                    LibreOffice.Viewer {
                        id: loView
                        objectName: "loView"
                        Layouts.item: "loView"

                        anchors.fill: parent

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

                        onErrorChanged: {
                            var errorString;

                            switch(error) {
                            case LibreOffice.Error.LibreOfficeNotFound:
                                errorString = i18n.tr("LibreOffice binaries not found.")
                                break;
                            case LibreOffice.Error.LibreOfficeNotInitialized:
                                errorString = i18n.tr("Error while loading LibreOffice.")
                                break;
                            case LibreOffice.Error.DocumentNotLoaded:
                                errorString = i18n.tr("Document not loaded.\nThe requested document may be corrupt.")
                                break;
                            }

                            if (errorString) {
                                loPage.pageStack.pop()

                                // We create the dialog in the MainView, so that it isn't
                                // initialized by 'loPage' and keep on working after the
                                // page is destroyed.
                                mainView.showErrorDialog(errorString);
                            }
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
        },

        LOViewZoomHeader {
            name: "zoom"
            targetPage: loPage
        }
    ]
}