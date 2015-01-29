/*
 * Copyright (C) 2013-2014 Canonical, Ltd.
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
import com.ubuntu.popplerqmlplugin 1.0

import "utils.js" as Utils

Page {
    id: pdfPage
    title: Utils.getNameOfFile(file.path);

    // Disable header auto-hide
    flickable: null

    property string currentPage: i18n.tr("Page %1 of %2").arg(pdfView.currentIndex + 1).arg(pdfView.count)

    // TODO: Restore zooming
    ListView {
        id: pdfView
        objectName:"pdfView"

        anchors {
            fill: parent
            leftMargin: units.gu(1)
            rightMargin: units.gu(1)
        }
        spacing: units.gu(2)

        clip: true
        focus: false
        boundsBehavior: Flickable.StopAtBounds

        cacheBuffer: height

        highlightFollowsCurrentItem: false
        keyNavigationWraps: false

        header: Item { width: parent.width; height: units.gu(2) }
        footer: Item { width: parent.width; height: units.gu(2) }

        model: Poppler {
            id: poppler

            /* FIXME: Don't set 'path' property directly, but set it through onCompleted signal.
               By doing otherwise, PDF pages are loaded two times, but only
               the first delegates are working. Asking to the image provider
               to get the second ones, makes the app instable.
               (e.g. We have a PDF document with 10 pages. The plugin loads
               them twice - 2x10 = 20 pages - but only the first 10 are shown.
               While trying to get the 11th, the app crashes). */
            Component.onCompleted: path = file.path

            onPagesLoaded: {
                activity.running = false;

                pdfView.currentIndex = 0

                var title = getDocumentInfo("Title")
                if (title !== "")
                    pdfPage.title = title
            }
        }

        delegate: PdfViewDelegate {}

        onWidthChanged: {
            /* On resizing window, pages size changes but contentY is still the same.
               For that reason, it shows the wrong page (which is settled at the same contentY).
               We need to force flickable to show the current page. */
            //pdfView.positionViewAtIndex(currentIndex, ListView.Contain)
        }

        onContentYChanged: {
            // FIXME: On wheeling up, ListView automatically center currentItem to the view.
            //        This causes some strange "jump" of ~200px in contentY
            var i = pdfView.indexAt(pdfView.width * 0.5, contentY + (pdfView.height * 0.5))

            if (i < 0) {
                // returned index could be -1 when the delegate spacing is shown at the center of the view (e.g. while scrolling pages)
                i = pdfView.indexAt(pdfView.width * 0.5, contentY + (pdfView.height * 0.5) + units.gu(4))
            }

            if (i !== -1) {
                currentPage = i18n.tr("Page %1 of %2").arg(i + 1).arg(pdfView.count)

                if (!pdfView.flickingVertically) {
                    pdfView.currentIndex = i
                }
            }
        }
    }

    ActivityIndicator {
        id: activity
        anchors.centerIn: parent

        running: true
    }

    // *** HEADER ***
    state: "default"
    states: [
        PdfViewDefaultHeader {
            name: "default"
            targetPage: pdfPage
        }
    ]
}
