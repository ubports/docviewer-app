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
 
AbstractButton {
    width: units.gu(4); height: parent.height
 
    property alias iconName: icon.name
    property alias iconSource: icon.source
 
    Icon {
        id: icon
        anchors.centerIn: parent
        width: units.gu(2.5); height: width
    }
}
