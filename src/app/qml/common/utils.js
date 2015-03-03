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

.pragma library

function printSize(size) {
    if (size >= 1073741824)
        return (size / 1073741824).toFixed(2) + qsTr(" GiB");

    if (size >= 1048576)
        return (size / 1048576).toFixed(2) + qsTr(" MiB");

    if (size >= 1024)
        return parseInt(size / 1024) + qsTr(" KiB");

    return size + qsTr(" byte");
}

function getNameOfFile(path) {
    return path.toString().substring(path.lastIndexOf('/') + 1);
}
