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

import QtQuick 2.4
import "../common"

Item {
    anchors.fill: parent

    EmptyState {
        title: i18n.tr("No matching document found")

        subTitle: i18n.tr("Please ensure that your query is not misspelled and/or try a different query.")
        iconName: "search"

        anchors.centerIn: parent
    }
}
