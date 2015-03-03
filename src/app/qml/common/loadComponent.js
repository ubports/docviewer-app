/*
 * Copyright (C) 2013-2015 Canonical, Ltd.
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

function load(mimetype) {
    var qmlToLoad = "";

    // Open all text files in text editor
    // With that fix it is possible to open LICENSE file
    // which was recognised as text/x-pascal
    if (mimetype.substring(0, 5) === "text/")
        qmlToLoad = Qt.resolvedUrl("../textView/TextView.qml");

    // Check if PDF document
    if (mimetype === "application/pdf")
        qmlToLoad = Qt.resolvedUrl("../pdfView/PdfView.qml");

    if (qmlToLoad != "") {
       pageStack.push(qmlToLoad);
    } else {
        console.debug("Unknown MIME type: "+ mimetype);
        runUnknownTypeDialog();
    }

    return mimetype;
}
