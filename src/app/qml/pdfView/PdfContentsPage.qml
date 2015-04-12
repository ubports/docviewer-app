/*
 * Copyright (C) 2014, 2015
 *                  Stefano Verzegnassi <verzegnassi.stefano@gmail.com>
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

import QtQuick 2.0
import Ubuntu.Components 1.1
import QtQuick.Layouts 1.1

import "../upstreamComponents"

Page {
    // TRANSLATORS: "Contents" refers to the "Table of Contents" of a PDF document.
    title: i18n.tr("Contents")

    // Avoid a binding loop when using ListView.positionViewAtIndex()
    flickable: null

    head.backAction: Action {
        text: i18n.tr("Hide table of contents")
        iconName: "down"
        onTriggered: pageStack.pop()
    }

    onActiveChanged: {
        // If the header was hidden in the PdfPage, make it visible.
        mainView.setHeaderVisibility(true);

        // Find out the current page position in the ToC index
        var i=0
        while(!(pdfView.currentPageIndex >= poppler.tocModel.get(i).pageIndex &&
              pdfView.currentPageIndex < poppler.tocModel.get(i+1).pageIndex)) {
            i++
        }

        // Set highlighted index
        view.currentIndex = i;

        // Position view at the highlighted index
        view.positionViewAtIndex(i, ListView.Center);
    }

    ListView {
        id: view
        anchors.fill: parent
        clip: true

        model: poppler.tocModel

        delegate: ListItemWithActions {
            id: delegate

            width: parent.width
            height: (model.level === 0) ? units.gu(7) : units.gu(6)

            // Don't use 'selected' property here, since it shows a CheckBox
            color: (view.currentIndex == model.index) ? Qt.lighter(UbuntuColors.lightGrey)
                                                      : Theme.palette.normal.background

            AbstractButton {
                anchors.fill: parent

                onClicked: {
                    pdfView.positionAtIndex(model.pageIndex);
                    pageStack.pop();
                }
            }

            RowLayout {
                anchors {
                    fill: parent
                    leftMargin: units.gu(1) + (units.gu(2) * model.level)
                    rightMargin: units.gu(1)
                }

                spacing: units.gu(1)

                Label {
                    Layout.fillWidth: true

                    text: model.title
                    elide: Text.ElideRight

                    font.weight: model.level == 0 ? Font.DemiBold : Font.Normal
                    color: (model.level === 0) ? UbuntuColors.midAubergine
                                               : Theme.palette.selected.backgroundText
                }

                Label {
                    text: model.pageIndex + 1
                    font.weight: model.level == 0 ? Font.DemiBold : Font.Normal
                    color: (model.level === 0) ? UbuntuColors.midAubergine
                                               : Theme.palette.selected.backgroundText
                }
            }

            Rectangle {
                anchors {
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom
                }

                height: units.gu(0.1)
                visible: model.level == 0
                color: (view.currentIndex === model.index) ? "transparent"
                                                           : UbuntuColors.midAubergine
            }
        }
    }
}
