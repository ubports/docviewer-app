/*
 * Copyright (C) 2013-2014 Canonical, Ltd.
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
import QtGraphicalEffects 1.0

Rectangle {
    id: pdfPage
    width: parent.width

    height: width * (model.height / model.width)

    border {
        width: 1
        color: "#808080"
    }

    Image {
        id: imagePage
        anchors {
            fill: parent
            margins: 1
        }
        asynchronous: true
        sourceSize.width: parent.width - 2
        fillMode: Image.PreserveAspectCrop

        Component.onCompleted: source = "image://poppler/page/" + model.index
    }

    Rectangle {
        anchors.fill: parent
        color: "white"
        visible: imagePage.status === Image.Loading

        ActivityIndicator {
            anchors.centerIn: parent
            running: parent.visible
        }
    }

    DropShadow {
        anchors.fill: parent
        cached: true;
        horizontalOffset: 0;
        verticalOffset: 2;
        radius: 8.0;
        samples: 16;
        color: "#80000000";
        smooth: true;
        source: parent;
        z: -10
    }
}
