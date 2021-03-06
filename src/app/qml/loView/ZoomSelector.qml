/*
 * Copyright (C) 2015 Stefano Verzegnassi
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
import QtQuick.Layouts 1.1
import DocumentViewer.LibreOffice 1.0 as LibreOffice

import "../common"

TextFieldWithButton {
    id: textField
    anchors.verticalCenter: parent.verticalCenter
    width: units.gu(12)

    property var view

    hasClearButton: true
    inputMethodHints: Qt.ImhFormattedNumbersOnly
    validator: IntValidator { bottom: 50; top: 400 }

    onAccepted: {
        textField.view.setZoom(parseInt(text) / 100)
        focus = false
    }

    onHighlightedChanged: {
        if (highlighted) {
            text = parseInt(textField.view.zoomSettings.zoomFactor * 100)
        } else text = ""
    }

    Label {
        anchors.centerIn: parent
        visible: !textField.highlighted
        text: "%1%".arg(parseInt(textField.view.zoomSettings.zoomFactor*100))
    }

    popover: TextFieldButtonPopover {
        id: zoomSelectorDialogue

        /************************************
        *    Zoom in - Zoom out controls    *
        ************************************/

        RowLayout {
            anchors { left: parent.left; right: parent.right }
            height: units.gu(4)
            spacing: units.gu(1)

            ListItem {
                divider.visible: false
                Layout.fillHeight: true
                Layout.fillWidth: true

                onClicked: textField.view.setZoom(textField.view.zoomSettings.zoomFactor + 0.1)

                Icon {
                    width: units.gu(2); height: width
                    anchors.centerIn: parent
                    name: "zoom-in"
                }
            }

            VerticalDivider {
                Layout.fillHeight: true
                Layout.preferredWidth: units.dp(2)
            }

            ListItem {
                divider.visible: false
                Layout.fillHeight: true
                Layout.fillWidth: true

                onClicked: textField.view.setZoom(textField.view.zoomSettings.zoomFactor - 0.1)

                Icon {
                    width: units.gu(2); height: width
                    anchors.centerIn: parent
                    name: "zoom-out"
                }
            }
        }   // RowLayout

        HorizontalDivider { anchors { left: parent.left; right: parent.right } }

        /************************************
        *       Zoom modes controls         *
        ************************************/

        Repeater {
            id: zoomModesRepeater

            function delegate_onClicked(mode) {
                if (mode === LibreOffice.Zoom.FitToWidth)
                    textField.view.adjustZoomToWidth()

                if (mode === LibreOffice.Zoom.FitToHeight)
                    textField.view.adjustZoomToHeight()

                if (mode === LibreOffice.Zoom.Automatic)
                    textField.view.adjustAutomaticZoom()
            }

            // Used for hiding the HorizontalDivider below.
            visible: view.zoomSettings.zoomModesAvailable > LibreOffice.Zoom.Manual

            model: [
                { text: i18n.tr("Fit width"),  mode: LibreOffice.Zoom.FitToWidth  },
                { text: i18n.tr("Fit height"), mode: LibreOffice.Zoom.FitToHeight },
                { text: i18n.tr("Automatic"),  mode: LibreOffice.Zoom.Automatic   }
            ]

            ListItem {
                height: units.gu(4)
                divider.visible: false

                visible: view.zoomSettings.zoomModesAvailable & modelData.mode

                onClicked: {
                    zoomSelectorDialogue.close()
                    zoomModesRepeater.delegate_onClicked(modelData.mode)
                }

                /* UITK 1.3 specs: Two slot layout (A-B) */
                ListItemLayout {
                    anchors.centerIn: parent

                    /* UITK 1.3 specs: Slot A */
                    title.text: modelData.text

                    /* UITK 1.3 specs: Slot B */
                    Icon {
                        SlotsLayout.position: SlotsLayout.Last
                        width: units.gu(2); height: width
                        name: "tick"
                        color: UbuntuColors.green
                        visible: textField.view.zoomSettings.zoomMode == modelData.mode
                    }
                }
            }   // ListItem
        }

        HorizontalDivider { visible: zoomModesRepeater.visible; anchors { left: parent.left; right: parent.right } }

        /************************************
        *   Default zoom values controls    *
        ************************************/

        Repeater {
            model: [
                { text: "50%",  value: 0.50 },
                { text: "70%",  value: 0.70 },
                { text: "85%",  value: 0.85 },
                { text: "100%", value: 1.00 },
                { text: "125%", value: 1.25 },
                { text: "150%", value: 1.50 },
                { text: "175%", value: 1.75 },
                { text: "200%", value: 2.00 },
                { text: "300%", value: 3.00 },
                { text: "400%", value: 4.00 }
            ]

            ListItem {
                divider.visible: false
                height: units.gu(4)

                onClicked: {
                    textField.view.setZoom(modelData.value)
                    zoomSelectorDialogue.close()
                }

                Label {
                    text: modelData.text
                    anchors {
                        left: parent.left; leftMargin: units.gu(1)
                        verticalCenter: parent.verticalCenter
                    }
                }
            }
        }   // Repeater
    }   // zoomSelectorDialogue
}   // textField
