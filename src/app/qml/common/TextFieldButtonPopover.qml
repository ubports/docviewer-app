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
import Ubuntu.Components.Popups 1.3

Popover {
    id: textFieldButtonPopover

    default property alias popoverData: layout.data

    signal close()

    autoClose: false
    contentHeight: layout.height + units.gu(4)
    contentWidth: units.gu(24)

    onClose: PopupUtils.close(textFieldButtonPopover)

    // We don't use 'autoClose' property, since we want to propagate
    // mouse/touch events to other items (e.g. when zoomSelectorDialogue
    // is visible, and user taps the zoom+ button on its right, we want
    // the zoom button to receive the event).
    InverseMouseArea {
        anchors.fill: parent
        propagateComposedEvents: true

        onPressed: {
            mouse.accepted = false
            PopupUtils.close(textFieldButtonPopover)
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
    }
}
