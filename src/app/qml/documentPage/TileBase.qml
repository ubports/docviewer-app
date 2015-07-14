/*
  Copyright (C) 2014, 2015 Canonical Ltd.

    This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License 3 as published by
  the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
  along with this program. If not, see http://www.gnu.org/licenses/.
*/

import QtQuick 2.0
import Ubuntu.Components 1.1
import Ubuntu.Components.Popups 1.0
import Ubuntu.Components.ListItems 1.0 as ListItem

AbstractButton {
    id: root

    property bool selected: false
    property bool selectionMode: false

    default property alias content: tileContent.data

    property alias title: titleLabel.text
    property alias text: textLabel.text
    property alias subText: subTextLabel.text

    // We don't really have swipe gesture here, but we anyway use the same
    // properties' name used for UITK1.2 ListItem just for consistency.
    property list<Action> trailingActions   // Right to Left gesture in ListItem
    property list<Action> leadingActions    // Left to Right gesture in ListItem

    Rectangle {
        id: rect

        anchors { fill: parent; margins: units.gu(0.5) }

        color: Qt.lighter(UbuntuColors.lightGrey)
        clip: true

        Item {
            id: tileContent

            height: parent.height - captionsLayout.height * 0.3 - units.gu(2)
            anchors {
                left: parent.left
                right: parent.right
                verticalCenter: parent.verticalCenter
                verticalCenterOffset: - captionsLayout.height * 0.3
            }
        }

        Loader {
            id: overflowButton

            anchors {
                right: parent.right
                top: parent.top
            }

            z: 10

            sourceComponent: actionsOverflowButton

            onStatusChanged: {
                if (status === Loader.Ready) {
                    item.iconName = "contextual-menu"
                    item.color = UbuntuColors.darkGrey
                    item.width = overflowButton.width
                    item.height = overflowButton.height

                    item.triggered.connect(function() {
                        var overflowPanel = PopupUtils.open(actionsOverflowPopoverComponent, item)
                        item.overflowPanelVisible = overflowPanel.visible;

                        overflowPanel.visibleChanged.connect(function() {
                            item.overflowPanelVisible = overflowPanel.visible
                        });
                    });
                }
            }

            width: units.gu(5)
            height: units.gu(5)
            visible: rect.state !== "select" && (trailingActions.length > 0 || leadingActions.length > 0)

            Behavior on opacity {
                NumberAnimation {
                    duration: UbuntuAnimation.SnapDuration
                }
            }
        }

        Loader {
            id: selectionIcon

            anchors {
                right: parent.right
                top: parent.top
            }

            z: 10

            width: (status === Loader.Ready) ? item.implicitWidth : 0
            visible: (status === Loader.Ready) && (item.width === item.implicitWidth)
            Behavior on opacity {
                NumberAnimation {
                    duration: UbuntuAnimation.SnapDuration
                }
            }
        }

        // Tile captions
        Rectangle {
            id: captionsRect

            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }

            height: captionsLayout.height + units.gu(1)

            color: UbuntuColors.darkGrey
            opacity: 0.75
            layer.enabled: true

            Column {
                id: captionsLayout
                anchors {
                    left: parent.left;
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                    margins: units.gu(0.5)
                }

                Label {
                    id: titleLabel
                    color: "white"

                    elide: Text.ElideRight
                    font.weight: Font.DemiBold
                    fontSize: "small"

                    anchors { left: parent.left; right: parent.right }
                }

                Label {
                    id: textLabel
                    color: "white"
                    fontSize: "small"

                    anchors { left: parent.left; right: parent.right }
                }

                Label {
                    id: subTextLabel
                    color: "white"
                    fontSize: "small"

                    anchors { left: parent.left; right: parent.right }
                }
            }
        }

        states: [
            State {
                name: "select"
                when: selectionMode || selected
                PropertyChanges {
                    target: selectionIcon
                    sourceComponent: selectionCheckBox
                    anchors.margins: units.gu(1)
                }
            }
        ]
    }

    // *** COMPONENTS
    Component {
        id: selectionCheckBox

        CheckBox {
            checked: root.selected
            width: implicitWidth
            // disable item mouse area to avoid conflicts with parent mouse area
            __mouseArea.enabled: false
        }
    }

    Component {
        id: actionsOverflowButton

        AbstractButton {
            id: button

            property alias color: icon.color
            property bool overflowPanelVisible: false

            width: visible ? units.gu(5) : 0
            height: parent ? parent.height : undefined
            Rectangle {
                visible: button.pressed || button.overflowPanelVisible
                anchors.fill: parent
                color: Theme.palette.selected.background
            }

            Icon {
                id: icon
                anchors.centerIn: parent

                // prevent trying to render the icon with an invalid source
                // when the button is invisible by setting width and height to 0
                width: visible ? units.gu(2.5) : 0
                height: visible ? units.gu(2.5) : 0
                source: button.iconSource
                color: Qt.rgba(0, 0, 0, 0)
                opacity: button.enabled ? 1.0 : 0.3
            }

            Component {
                id: labelComponent
                Label {
                    id: label
                    objectName: button.objectName + "_label"
                    color: button.color
                    opacity: button.enabled ? 1.0 : 0.3
                    text: button.text
                    fontSize: "xx-small"
                }
            }
            Loader {
                anchors {
                    top: icon.bottom
                    topMargin: units.gu(0.5)
                    horizontalCenter: parent.horizontalCenter
                }
                sourceComponent: button.state === "IconAndLabel" ? labelComponent : null
            }
        }
    }

    Component {
        id: actionsOverflowPopoverComponent

        Popover {
            id: actionsOverflowPopover
            property bool square: true
            callerMargin: -units.gu(1) + units.dp(4)
            contentWidth: units.gu(20)
            contentHeight: popoverActionsLayout.height

            Connections {
                target: root
                onLeadingActionsChanged: {
                    actionsOverflowPopover.hide();
                }
                onTrailingActionsChanged: {
                    actionsOverflowPopover.hide();
                }
            }

            Column {
                id: popoverActionsLayout
                anchors {
                    left: parent.left
                    top: parent.top
                    right: parent.right
                }
                Repeater {
                    id: overflowTrailingRepeater
                    model: root.trailingActions

                    delegate: overflowPanelDelegate
                    onItemAdded: {
                        item.action = model[index]
                        item.clicked.connect(function() {
                            actionsOverflowPopover.hide()
                        })

                        item.showDivider = (overflowLeadingRepeater.count === 0) ? (index !== overflowTrailingRepeater.count - 1) : true
                    }
                }

                Repeater {
                    id: overflowLeadingRepeater
                    model: root.leadingActions

                    delegate: overflowPanelDelegate
                    onItemAdded: {
                        item.action = model[index]
                        item.clicked.connect(function() {
                            actionsOverflowPopover.hide()
                        })

                        item.showDivider = (index !== overflowLeadingRepeater.count - 1)
                    }
                }
            }
        }
    }

    Component {
        id: overflowPanelDelegate

        AbstractButton {
            id: rootItem
            implicitHeight: units.gu(6) + bottomDividerLine.height
            width: parent ? parent.width : units.gu(31)

            property bool showDivider: true
            property color foregroundColor: Theme.palette.selected.backgroundText

            Rectangle {
                visible: parent.pressed
                anchors {
                    left: parent.left
                    right: parent.right
                    top: parent.top
                }
                height: parent.height - bottomDividerLine.height
                color: Theme.palette.selected.background
            }

            Icon {
                id: actionIcon
                source: action.iconSource
                color: rootItem.foregroundColor
                anchors {
                    verticalCenter: parent.verticalCenter
                    verticalCenterOffset: units.dp(-1)
                    left: parent.left
                    leftMargin: units.gu(2)
                }
                width: units.gu(2)
                height: units.gu(2)
                opacity: action.enabled ? 1.0 : 0.5
            }

            Label {
                anchors {
                    verticalCenter: parent.verticalCenter
                    verticalCenterOffset: units.dp(-1)
                    left: actionIcon.right
                    leftMargin: units.gu(2)
                    right: parent.right
                }
                fontSize: "small"
                elide: Text.ElideRight
                text: action.text
                color: rootItem.foregroundColor
                opacity: action.enabled ? 1.0 : 0.5
            }

            ListItem.ThinDivider {
                id: bottomDividerLine
                anchors.bottom: parent.bottom
                visible: rootItem.showDivider
            }
        }
    }
}
