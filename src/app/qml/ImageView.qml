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

import QtQuick 2.3
import Ubuntu.Components 1.1

import "utils.js" as Utils

Page {
    id: imagePage
    title: Utils.getNameOfFile(file.path);

    Rectangle {
        anchors.fill: parent
        color: "#221E1C"    // SuruDark.Palette.Normal.Background
    }

    ZoomableImage {
        id: zoomableImage
        anchors.fill: parent

        zoomable: true
        source: file.path
    }

    // *** HEADER ***
    state: "default"
    states: [
        ImageViewDefaultHeader {
            name: "default"
            targetPage: imagePage
            activityRunning: zoomableImage.imageStatus == Image.Loading
        }
    ]
}
