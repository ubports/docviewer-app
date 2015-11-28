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

Item {
    width: units.dp(2)

    Rectangle {
        height: parent.width
        width: parent.height
        rotation: 90; anchors.centerIn: parent

        gradient: Gradient {
            GradientStop { position: 0.0; color: Qt.rgba(0, 0, 0, 0.1)  }
            GradientStop { position: 0.49; color:  Qt.rgba(0, 0, 0, 0.1)  }
            GradientStop { position: 0.5; color: Qt.rgba(1, 1, 1, 0.4) }
            GradientStop { position: 1.0; color:  Qt.rgba(1, 1, 1, 0.4)  }
        }
    }
}
