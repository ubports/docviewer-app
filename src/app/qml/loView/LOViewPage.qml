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
import DocumentViewer 1.0
import DocumentViewer.LibreOffice 1.0 as LibreOffice

import "../common"
import "../common/utils.js" as Utils
import "../common"
import "KeybHelper.js" as KeybHelper

ViewerPage {
    id: loPage

    property bool isPresentation: loPage.contentItem && (loPage.contentItem.loDocument.documentType === LibreOffice.Document.PresentationDocument)
    property bool isTextDocument: loPage.contentItem && (loPage.contentItem.loDocument.documentType === LibreOffice.Document.TextDocument)
    property bool isSpreadsheet: loPage.contentItem && (loPage.contentItem.loDocument.documentType === LibreOffice.Document.SpreadsheetDocument)

    title: DocumentViewer.getFileBaseNameFromPath(file.path);
    flickable: isTextDocument ? loPage.contentItem.loView : null

    splashScreen: Splashscreen { }

    content: FocusScope {
        id: loPageContent
        anchors.fill: parent

        property alias loDocument: loView.document
        property alias loView: loView

        Layouts {
            id: layouts
            anchors.fill: parent

            layouts: [
                ConditionalLayout {
                    when: mainView.veryWideWindow
                    name: "wideWindowLayout"

                    Item {
                        anchors.fill: parent

                        ResizeableSidebar {
                            id: leftSidebar
                            anchors.left: parent.left
                            anchors.bottom: parent.bottom
                            visible: loPage.isPresentation

                            width: visible ? units.gu(40) : 0

                            PartsView {
                                id: partsView
                                anchors.fill: parent
                                model: loView.partsModel
                            }
                        }

                        ItemLayout {
                            item: "pinchArea"
                            anchors {
                                left: leftSidebar.right
                                right: parent.right
                                top: parent.top
                                bottom: sSelector.top
                            }
                        }

                        SpreadsheetSelector {
                            id: sSelector
                            anchors.bottom: parent.bottom
                            visible: loPage.isSpreadsheet
                            view: loView
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
                    anchors.fill: parent

                    clip: true
                    documentPath: file.path

                    function updateContentSize(tgtScale) {
                        zoomFactor = tgtScale
                    }

                    // Keyboard events
                    focus: true
                    Keys.onPressed: KeybHelper.parseEvent(event)

                    Component.onCompleted: {
                        // WORKAROUND: Fix for wrong grid unit size
                        flickDeceleration = 1500 * units.gridUnit / 8
                        maximumFlickVelocity = 2500 * units.gridUnit / 8
                        loPageContent.forceActiveFocus()
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
                            errorString = i18n.tr("Document not loaded.\nThe requested document may be corrupt or protected by a password.")
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

            Item {
                id: bottomBar
                anchors {
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom
                }
                height: childrenRect.height

                PartsView {
                    anchors { left: parent.left; right: parent.right }
                    height: visible ? units.gu(12) : 0
                    visible: loPage.isPresentation

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

                SpreadsheetSelector {
                    visible: loPage.isSpreadsheet
                    view: loView
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
