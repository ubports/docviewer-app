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

import QtQuick 2.3
import Ubuntu.Components 1.1

Item {
    id: rootItem

    property string documentFile: ""
    property string documentsDir: ""
    property bool fullscreen: false
    property bool pickMode: false

    Component.onCompleted: {
        var docFile = args.defaultArgument.at(0)
        if (docFile)
            rootItem.documentFile = docFile

        var docsDir = args.values.documentsDir
        if (docsDir)
            rootItem.documentsDir = docsDir

        var fullscreen = args.values.fullscreen
        if (fullscreen)
            rootItem.fullscreen = fullscreen

        var pickMode =  args.values.pickMode
        if (pickMode)
            rootItem.pickMode = pickMode

        // Error catching
        // FIXME: Not working
        if (args.error) {
            var errorString = i18n.tr("Some of the provided arguments is not valid.")
            args.quitWithError(errorString)
        }
    }

    Arguments {
        id: args

        defaultArgument {
            help: i18n.tr("Opens ubuntu-docviewer-app displaying the selected file")
            valueNames: ["file_path"]
            required: false
        }

        Argument {
            name: "fullscreen"
            help: i18n.tr("Run fullscreen")
            required: false
        }

        Argument {
            name: "pickMode"
            help: i18n.tr("Open ubuntu-docviewer-app in pick mode. Use it for tests only.")
            required: false
        }

        Argument {
            name: "documentsDir"
            help: i18n.tr("Load the list of documents from the given folder, instead of default ~/Documents.\nThe path must exist prior to running ubuntu-docviewer-app")
            valueNames: ["PATH"]
            required: false
        }
    }
}
