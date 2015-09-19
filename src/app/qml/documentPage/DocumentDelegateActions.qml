/*
  Copyright (C) 2015 Canonical Ltd.

    This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License 3 as published by
  the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
  along with this program. If not, see http://www.gnu.org/licenses/.
*/

import QtQuick 2.0
import Ubuntu.Components 1.1
import Ubuntu.Components.Popups 1.0

QtObject {
    property list<Action> leadingActions: [
        Action {
            iconName: "delete"
            text: i18n.tr("Delete")
            onTriggered: {
                var dialog = PopupUtils.open(Qt.resolvedUrl("DeleteFileDialog.qml"),
                                documentPage, { path: model.path })

                dialog.Component.destruction.connect(function() {
                    if (dialog.confirmed) {
                        console.log("Removing:", model.path);
                        docModel.rm(model.path);
                    }
                });
            }
        }
    ]

    property list<Action> trailingActions: [
        Action {
            iconName: "share"
            text: i18n.tr("Share")
            onTriggered: {
                pageStack.push(Qt.resolvedUrl("./SharePage.qml"), { fileUrl: model.path })
            }
        }
    ]
}
