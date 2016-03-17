/*
 * Copyright (C) 2015, 2016 Stefano Verzegnassi <verzegnassi.stefano@gmail.com>
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

// A page that loads its content asynchronously.

Page {
    id: viewerPage

    property alias splashScreen: splashScreenItem.data

    property alias content: contentLoader.sourceComponent
    property alias contentItem: contentLoader.item

    property bool  loaded: contentLoader.status == Loader.Ready
    property alias loaderProgress: contentLoader.progress
    property alias loaderStatus: contentLoader.status

    signal contentLoaded()

    Loader {
        id: contentLoader
        anchors {
            top: {
                if (viewerPage.header && !viewerPage.header.flickable)
                    return viewerPage.header.bottom
                else
                    return parent.top
            }

            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        asynchronous: true
        sourceComponent: viewerPage.contents

        onLoaded: viewerPage.contentLoaded()
    }

    Item {
        id: splashScreenItem
        anchors {
            top: {
                if (viewerPage.header && !viewerPage.header.flickable)
                    return viewerPage.header.bottom
                else
                    return parent.top
            }

            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        visible: contentLoader.status != Loader.Ready
        enabled: visible
    }
}
