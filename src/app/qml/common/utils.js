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

function printSize(i18n, size) {
    if (size >= 1073741824)
        // TRANSLATORS: %1 is the size of a file, expressed in GB
        return i18n.tr("%1 GB").arg((size / 1073741824).toFixed(2));

    if (size >= 1048576)
        // TRANSLATORS: %1 is the size of a file, expressed in MB
        return i18n.tr("%1 MB").arg((size / 1048576).toFixed(2));

    if (size >= 1024)
        // TRANSLATORS: %1 is the size of a file, expressed in kB
        return i18n.tr("%1 kB").arg(parseInt(size / 1024));

    // TRANSLATORS: %1 is the size of a file, expressed in byte
    return i18n.tr("%1 byte").arg(size);
}

function getNameOfFile(path) {
    return path.toString().substring(path.lastIndexOf('/') + 1);
}
