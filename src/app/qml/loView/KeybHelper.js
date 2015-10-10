/*
 * Copyright (C) 2015 Stefano Verzegnassi
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
 
function parseEvent(event) {
    var pixelDiff = 5;
 
    if (event.key == Qt.Key_PageUp) {
        if (loDocument.documentType == LO.Document.PresentationDocument)
            loDocument.currentPart -= 1
        else
            loPage.moveView("vertical", -loView.height)
 
        return;
    }
 
    if (event.key == Qt.Key_PageDown) {
        if (loDocument.documentType == LO.Document.PresentationDocument)
            loDocument.currentPart += 1
        else
            loPage.moveView("vertical", loView.height)
 
        return;
    }
 
    if (event.key == Qt.Key_Up) {
        loPage.moveView("vertical", -pixelDiff)
        return;
    }
 
    if (event.key == Qt.Key_Down) {
        loPage.moveView("vertical", pixelDiff)
        return;
    }
 
    if (event.key == Qt.Key_Left) {
        loPage.moveView("horizontal", -pixelDiff)
        return;
    }
 
    if (event.key == Qt.Key_Right) {
        loPage.moveView("horizontal", pixelDiff)
        return;
    }
}
