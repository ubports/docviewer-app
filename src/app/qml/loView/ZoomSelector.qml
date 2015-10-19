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
import Ubuntu.Components 1.2
import Ubuntu.Components.Popups 1.0
import DocumentViewer.LibreOffice 1.0 as LibreOffice

TextField {
    id: textField
    anchors.verticalCenter: parent.verticalCenter
    width: units.gu(24)

    property var values: ["auto", 0.5, 0.7, 0.85, 1.0, 1.25, 1.5, 1.75, 2.0, 3.0, 4.0]
    property var labels: [
        i18n.tr("Automatic (Fit width)"),
        "50%", "70%", "85%", "100%", "125%",
        "150%", "175%","200%", "300%", "400%"
    ]

    property bool expanded: false

    hasClearButton: true
    inputMethodHints: Qt.ImhFormattedNumbersOnly
    validator: IntValidator{ bottom: 50; top: 400 }

    onAccepted: {
        loPageContentLoader.item.loView.zoomFactor = parseInt(text) / 100
        focus = false
    }

    secondaryItem: AbstractButton {
        visible: !textField.highlighted
        id: listButton
        height: parent.height
        width: visible ? height : 0

        Rectangle {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height - units.gu(1)

            width: units.dp(1)
            color: "Grey"
            opacity: 0.5
        }

        Icon {
            id: _upArrow

            width: units.gu(2)
            height: width
            anchors.centerIn: parent

            name: "go-down"
            color: "Grey"
            rotation: textField.expanded ? 180 : 0

            Behavior on rotation {
                UbuntuNumberAnimation {}
            }
        }

        onClicked: {
            textField.expanded = true
            PopupUtils.open(zoomSelectorDialog, listButton)
        }
    }

    onHighlightedChanged: {
        if (highlighted) {
            text = parseInt(loPageContentLoader.item.loView.zoomFactor * 100)
        } else text = ""
    }

    Label {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: units.gu(2)
        visible: !textField.highlighted
        text: loPageContentLoader.item.loView.zoomMode == LibreOffice.View.FitToWidth ? i18n.tr("Automatic (%1%)").arg(parseInt(loPageContentLoader.item.loView.zoomFactor*100))
                                                             : i18n.tr("Zoom: %1%").arg(parseInt(loPageContentLoader.item.loView.zoomFactor*100))
    }

    Component {
        id: zoomSelectorDialog
        Popover {
            id: zoomSelectorDialogue
            autoClose: false
            contentHeight: units.gu(24)
            contentWidth: units.gu(24)
            Component.onDestruction: textField.expanded = false

            // We don't use 'autoClose' property, since we want to propagate
            // mouse/touch events to other items (e.g. when zoomSelectorDialogue
            // is visible, and user taps the zoom+ button on its right, we want
            // the zoom button to receive the event).
            InverseMouseArea {
                anchors.fill: parent
                propagateComposedEvents: true

                onPressed: {
                    mouse.accepted = false
                    PopupUtils.close(zoomSelectorDialogue)
                }
            }

            OptionSelector {
                expanded: true
                width: parent.width
                containerHeight: units.gu(24)
                model: textField.labels
                selectedIndex: {
                    if (loPageContentLoader.item.loView.zoomMode == LibreOffice.View.FitToWidth)
                        return 0

                    for (var i=0; i<textField.values.length; i++) {
                        if (values[i] == loView.zoomFactor)
                            return i
                    }
                    return -1
                }

                onSelectedIndexChanged: {
                    if (selectedIndex == 0) {
                        loView.adjustZoomToWidth();
                        return;
                    }

                    loPageContentLoader.item.loView.zoomFactor = textField.values[selectedIndex]
                    PopupUtils.close(zoomSelectorDialogue)
                }
            }
        }
    }
}
