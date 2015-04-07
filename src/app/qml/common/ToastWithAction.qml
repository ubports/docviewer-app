/*
  Copyright (C) 2014, 2015 Stefano Verzegnassi

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
import QtQuick.Layouts 1.1

Rectangle {
    id: rootItem

    property alias text: label.text
    readonly property alias action: action

    width: parent.width
    height: units.gu(8)

    color: "#131313"
    opacity: 0.85
    layer.enabled: true

    anchors {
        horizontalCenter: parent.horizontalCenter
        bottom: parent.bottom; bottomMargin: - height
    }

    RowLayout {
        anchors {
            fill: parent
            margins: units.gu(2)
        }

        Label {
            id: label
            Layout.fillWidth: true

            font.weight: Font.DemiBold
            color: "white"
        }

        AbstractButton {
            width: actionLabel.paintedWidth
            height: parent.height

            onClicked: action.triggered()

            Label {
                id: actionLabel
                text: action.text

                font.capitalization: Font.AllUppercase
                font.weight: Font.DemiBold
                color: UbuntuColors.orange

                anchors.centerIn: parent
            }
        }
    }

    MouseArea {
        anchors.fill: parent

        onClicked: {
            showAnimation.stop()
            destroyAnimation.restart()
        }
    }

    Rectangle {
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        height: units.dp(2)
        color: UbuntuColors.orange
    }

    Action {
        id: action

        text: i18n.tr("Dismiss")
        onTriggered: destroyAnimation.restart()
    }

    SequentialAnimation {
        id: showAnimation
        running: true

        NumberAnimation { target: rootItem; property: "anchors.bottomMargin"; to: 0; duration: 300 }
        PauseAnimation { duration: 2000 }
        ScriptAction { script: destroyAnimation.restart() }
    }

    SequentialAnimation {
        id: destroyAnimation

        NumberAnimation { target: rootItem; property: "opacity"; to: 0; duration: 500 }
        ScriptAction { script: rootItem.destroy() }
    }
}

