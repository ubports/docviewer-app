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
import Ubuntu.Components.ListItems 1.0 as ListItem

Page {
    title: i18n.tr("Contents")

    ListView {
        anchors.fill: parent

        model: poppler.tocModel

        delegate: ListItem.Base {
            showDivider: model.level == 0

            onClicked: {
                pdfView.positionAtIndex(model.pageIndex);
                pageStack.pop();
            }

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: units.gu(2) * model.level

                spacing: units.gu(1)

                Label {
                    Layout.fillWidth: true

                    text: (typeof model.title === "undefined") ? "" : model.title;
                    elide: Text.ElideRight
                    Component.onCompleted: { if (model.level === 0); font.weight = Font.DemiBold; }
                }

                Label {
                    text: (typeof model.pageIndex === "undefined") ? "" : model.pageIndex + 1;
                    Component.onCompleted: { if (model.level === 0); font.weight = Font.DemiBold; }
                }
            }
        }
    }
}
