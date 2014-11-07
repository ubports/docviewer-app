/*
 * Copyright (C) 2012-2014 Canonical, Ltd.
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
import Ubuntu.Content 1.1 as ContentHub

QtObject {
    property QtObject pageStack: null
    property list<QtObject> objects: [
        Connections {
            target: ContentHub.ContentHub

            onImportRequested: {
                if (transfer.state === ContentHub.ContentTransfer.Charged) {
                    // We have no signals to know if an import was requested before Component.completed signal
                    //  is emitted. So clear the stack when this occurs.
                    pageStack.clear()

                    console.log("[CONTENT-HUB] Incoming Import Request")
                    file.path = transfer.items[0].url.toString().replace("file://", "");
                }
            }
        }
    ]
}
