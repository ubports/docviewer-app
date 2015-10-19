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
import Ubuntu.Components 1.3
import QtQuick.Layouts 1.1
import Ubuntu.Components.Popups 1.0
import Ubuntu.Components.ListItems 1.0 as ListItems
import DocumentViewer.LibreOffice 1.0 as LibreOffice
import Ubuntu.Components.Themes.Ambiance 1.3 as Theme

TextField {
    id: textField
    anchors.verticalCenter: parent.verticalCenter
    width: units.gu(12)

    property bool expanded: false
    property var view: loPageContentLoader.item.loView

    hasClearButton: true
    inputMethodHints: Qt.ImhFormattedNumbersOnly
    validator: IntValidator { bottom: 50; top: 400 }

    secondaryItem: AbstractButton {
        id: listButton
        height: parent.height
        visible: !textField.highlighted
        width: visible ? height : 0

        onClicked: {
            textField.expanded = true
            PopupUtils.open(zoomSelectorDialog, listButton)
        }

        Rectangle {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height

            width: units.dp(1)
            color: theme.palette.selected.background
        }

        Rectangle {
            anchors.fill: parent
            color: theme.palette.selected.background
            visible: textField.expanded
        }

        Icon {
            width: units.gu(2); height: width
            anchors.centerIn: parent

            name: "go-down"
            color: "Grey"
            rotation: textField.expanded ? 180 : 0

            Behavior on rotation {
                UbuntuNumberAnimation {}
            }
        }
    }

    onAccepted: {
        view.setZoom(parseInt(text) / 100)
        focus = false
    }

    onHighlightedChanged: {
        if (highlighted) {
            text = parseInt(view.zoomFactor * 100)
        } else text = ""
    }

    Label {
        anchors.centerIn: parent
        anchors.horizontalCenterOffset: - listButton.width * 0.5

        visible: !textField.highlighted
        text: "%1%".arg(parseInt(view.zoomFactor*100))
    }

    Component {
        id: zoomSelectorDialog
        Popover {
            id: zoomSelectorDialogue
            autoClose: false
            contentHeight: layout.height + units.gu(4)
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

            Column {
                id: layout
                spacing: units.gu(0.5)
                anchors {
                    top: parent.top
                    left: parent.left
                    right: parent.right
                    margins: units.gu(2)
                }

                RowLayout {
                    anchors { left: parent.left; right: parent.right }
                    height: units.gu(4)
                    spacing: units.gu(1)

                    ListItems.Base {
                        showDivider: false
                        Layout.fillHeight: true
                        Layout.fillWidth: true

                        onClicked: {
                            var view = loPageContentLoader.item.loView
                            view.setZoom(view.zoomFactor + 0.1)
                        }

                        Icon {
                            width: units.gu(2); height: width
                            anchors.centerIn: parent
                            name: "zoom-in"
                        }
                    }

                    Rectangle {
                        Layout.fillHeight: true
                        Layout.preferredWidth: units.dp(1)
                        color: Theme.palette.selected.background
                    }

                    ListItems.Base {
                        showDivider: false
                        Layout.fillHeight: true
                        Layout.fillWidth: true

                        onClicked: {
                            var view = loPageContentLoader.item.loView
                            view.setZoom(view.zoomFactor - 0.1)
                        }

                        Icon {
                            width: units.gu(2); height: width
                            anchors.centerIn: parent
                            name: "zoom-out"
                        }
                    }
                }

                Rectangle {
                    anchors { left: parent.left; right: parent.right }
                    height: units.dp(1)
                    color: Theme.palette.selected.background
                }

                ListItems.Standard {
                    showDivider: false
                    height: units.gu(4)
                    __foregroundColor: Theme.palette.selected.backgroundText

                    text: i18n.tr("Fit width")
                    control: Icon {
                        width: units.gu(2); height: width
                        name: "tick"
                        color: UbuntuColors.green
                        visible: view.zoomMode == LibreOffice.View.FitToWidth
                    }

                    onClicked: {
                        view.adjustZoomToWidth()
                        PopupUtils.close(zoomSelectorDialogue)
                    }
                }

                Rectangle {
                    anchors { left: parent.left; right: parent.right }
                    height: units.dp(1)
                    color: Theme.palette.selected.background
                }

                ListItems.Standard {
                    showDivider: false
                    height: units.gu(4)
                    __foregroundColor: Theme.palette.selected.backgroundText

                    text: "50%"
                    onClicked: {
                        view.setZoom(0.5)
                        PopupUtils.close(zoomSelectorDialogue)
                    }
                }
                ListItems.Standard {
                    showDivider: false
                    height: units.gu(4)
                    __foregroundColor: Theme.palette.selected.backgroundText

                    text: "70%"
                    onClicked: {
                        view.setZoom(0.7)
                        PopupUtils.close(zoomSelectorDialogue)
                    }
                }
                ListItems.Standard {
                    showDivider: false
                    height: units.gu(4)
                    __foregroundColor: Theme.palette.selected.backgroundText

                    text: "85%"
                    onClicked: {
                        view.setZoom(0.85)
                        PopupUtils.close(zoomSelectorDialogue)
                    }
                }
                ListItems.Standard {
                    showDivider: false
                    height: units.gu(4)
                    __foregroundColor: Theme.palette.selected.backgroundText

                    text: "100%"
                    onClicked: {
                        view.setZoom(1.0)
                        PopupUtils.close(zoomSelectorDialogue)
                    }
                }
                ListItems.Standard {
                    showDivider: false
                    height: units.gu(4)
                    __foregroundColor: Theme.palette.selected.backgroundText

                    text: "125%"
                    onClicked: {
                        view.setZoom(1.25)
                        PopupUtils.close(zoomSelectorDialogue)
                    }
                }
                ListItems.Standard {
                    showDivider: false
                    height: units.gu(4)
                    __foregroundColor: Theme.palette.selected.backgroundText

                    text: "150%"
                    onClicked: {
                        view.setZoom(1.5)
                        PopupUtils.close(zoomSelectorDialogue)
                    }
                }
                ListItems.Standard {
                    showDivider: false
                    height: units.gu(4)
                    __foregroundColor: Theme.palette.selected.backgroundText

                    text: "175%"
                    onClicked: {
                        view.setZoom(1.75)
                        PopupUtils.close(zoomSelectorDialogue)
                    }
                }
                ListItems.Standard {
                    showDivider: false
                    height: units.gu(4)
                    __foregroundColor: Theme.palette.selected.backgroundText

                    text: "200%"
                    onClicked: {
                        view.setZoom(2.0)
                        PopupUtils.close(zoomSelectorDialogue)
                    }
                }
                ListItems.Standard {
                    showDivider: false
                    height: units.gu(4)
                    __foregroundColor: Theme.palette.selected.backgroundText

                    text: "300%"
                    onClicked: {
                        view.setZoom(3.0)
                        PopupUtils.close(zoomSelectorDialogue)
                    }
                }
                ListItems.Standard {
                    showDivider: false
                    height: units.gu(4)
                    __foregroundColor: Theme.palette.selected.backgroundText

                    text: "400%"
                    onClicked: {
                        view.setZoom(4.0)
                        PopupUtils.close(zoomSelectorDialogue)
                    }
                }
            }   // layout
        }   // zoomSelectorDialogue
    }   // zoomSelectorDialog

    /* style */
    style: Theme.TextFieldStyle {
        background: UbuntuShape {
            anchors.fill: parent
            aspect: UbuntuShape.DropShadow
        }
    }

}   // textField
