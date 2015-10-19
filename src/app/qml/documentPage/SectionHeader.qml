import QtQuick 2.4
import Ubuntu.Components 1.3
import Ubuntu.Components.ListItems 1.0 as ListItem
import DocumentViewer 1.0

ListItem.Header {
    text: {
        if (sortSettings.sortMode === 1)    // sort by name
            return section.toUpperCase()

        if (sortSettings.sortMode === 0) {    // sort by date
            if (section == DocumentsModel.Today)
                return i18n.tr("Today")

            if (section == DocumentsModel.Yesterday)
                return i18n.tr("Yesterday")

            if (section == DocumentsModel.LastWeek)
                return i18n.tr("Earlier this week")

            if (section == DocumentsModel.LastMonth)
                return i18n.tr("Earlier this month")

            return i18n.tr("Even earlier...")
        }

        return ""
    }
}
