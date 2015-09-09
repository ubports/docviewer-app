/*
  Copyright (C) 2013-2015 Stefano Verzegnassi

    This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License 3 as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

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

Dialog {
    id: deleteFileDialog

    property string path
    property int __deleteCount: documentPage.view.item.selectedItems.count

    // WORKAROUND: This property is used only when user wants to remove a single
    // file from a delegate action, and the value of the property is read during
    // the Component.onDestruction event.
    // We do this because we need to avoid that the entry in the model is removed
    // before this dialog is closed.
    // See src/app/qml/documentPage/DocumentDelegateActions.qml
    property bool confirmed: false

    title: path ? i18n.tr("Delete file") :
                  i18n.tr("Delete %1 file", "Delete %1 files", __deleteCount).arg(__deleteCount)
    text: path ? i18n.tr("Are you sure you want to permanently delete this file?") :
                 i18n.tr("Are you sure you want to permanently delete this file?",
                            "Are you sure you want to permanently delete these files?",
                            __deleteCount)

    Button {
        text: i18n.tr("Cancel")
        onClicked: PopupUtils.close(deleteFileDialog)
    }

    Button {
        text: i18n.tr("Delete")
        color: UbuntuColors.red

        onClicked: {
            if (deleteFileDialog.path) {
               deleteFileDialog.confirmed = true;
            } else {
                // This is called from selection mode
                var items = documentPage.view.item.selectedItems;

                for (var i=0; i < items.count; i++) {
                    console.log("Removing:", items.get(i).model.path);
                    docModel.rm(items.get(i).model.path);
                }

                viewLoader.item.endSelection();
            }

            PopupUtils.close(deleteFileDialog)
        }
    }
}

