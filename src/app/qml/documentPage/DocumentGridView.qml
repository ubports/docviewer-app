/*
  Copyright (C) 2015 Stefano Verzegnassi

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

import "../upstreamComponents"

MultipleSelectionGridView {
    id: documentGridView

    // We use mainView.width to calculate the size and the spacing of elements.
    // That's because this GridView always fills (and always will) the whole size of MainView.
    // By this way, we can avoid binding loops, keeping the code pretty simple.
    anchors {
        fill: parent
        margins: units.gu(0.5)
        leftMargin: (mainView.width % cellWidth) * 0.5
        rightMargin: (mainView.width % cellWidth) * 0.5
    }
    clip: true

    cellHeight: cellWidth
    cellWidth: (mainView.width > units.gu(50)) ? units.gu(24)
                                               : (mainView.width - units.gu(2)) * 0.5

    listDelegate: DocumentGridDelegate {
        id: delegate
        width: cellWidth
        height: cellHeight

        selectionMode: documentGridView.isInSelectionMode
        selected: documentGridView.isSelected(delegate)

        onClicked: {
            if(documentGridView.isInSelectionMode) {
                if(!documentGridView.selectItem(delegate)) {
                    documentGridView.deselectItem(delegate)
                }
                return
            }
            else {
                file.path = model.path
            }
        }

        onPressAndHold: {
            if (!documentGridView.isInSelectionMode) {
                documentGridView.startSelection()
                documentGridView.selectItem(delegate)
            }
        }
    }

    listModel: folderModel

    Scrollbar {
        flickableItem: documentGridView
        parent: documentGridView.parent
    }

    Component.onCompleted: { if (DOC_VIEWER.pickModeEnabled) documentGridView.startSelection(); }
}
