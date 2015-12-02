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

    var view = loPage.contentItem.loView
    var isPresentation = view.document.documentType === LibreOffice.Document.PresentationDocument

    if (event.key == Qt.Key_PageUp) {
        if (isPresentation)
            view.document.currentPart -= 1
        else
            view.moveView("vertical", -view.height)
 
        return;
    }
 
    if (event.key == Qt.Key_PageDown) {
        if (isPresentation)
            view.document.currentPart += 1
        else
            view.moveView("vertical", view.height)
 
        return;
    }
 
    if (event.key == Qt.Key_Home) {
        if (event.modifiers & Qt.ControlModifier) {
            view.contentX = 0
            view.contentY = 0
            view.document.currentPart = 0
        } else {
            view.contentX = 0
            view.contentY = 0
        }
    }

    if (event.key == Qt.Key_End) {
        if (event.modifiers & Qt.ControlModifier) {
            view.contentX = view.contentWidth - view.width
            view.contentY = view.contentHeight - view.height
            console.log(view.document.currentPart, view.document.partsCount - 1)
            view.document.currentPart = view.document.partsCount - 1
        } else {
            view.contentX = view.contentWidth - view.width
            view.contentY = view.contentHeight - view.height
        }
    }

    if (event.key == Qt.Key_Up) {
        view.moveView("vertical", -pixelDiff)
        return;
    }
 
    if (event.key == Qt.Key_Down) {
        view.moveView("vertical", pixelDiff)
        return;
    }
 
    if (event.key == Qt.Key_Left) {
        view.moveView("horizontal", -pixelDiff)
        return;
    }
 
    if (event.key == Qt.Key_Right) {
        view.moveView("horizontal", pixelDiff)
        return;
    }

    if (event.key == Qt.Key_Plus) {
        if (event.modifiers & Qt.ControlModifier) {
            view.zoomFactor = Math.max(4.0, view.zoomFactor + 0.25)
        }
    }

    if (event.key == Qt.Key_Minus) {
        if (event.modifiers & Qt.ControlModifier) {
            view.zoomFactor = Math.min(0.5, view.zoomFactor - 0.25)
        }
    }


    /*
    if (event.key == Qt.Key_C) {
        if (event.modifiers & Qt.ControlModifier) {

        }
    }
    */

    /*
    if (event.key == Qt.Key_X) {
        if (event.modifiers & Qt.ControlModifier) {

        }
    }
    */

    /*
    if (event.key == Qt.Key_V) {
        if (event.modifiers & Qt.ControlModifier) {

        }
    }
    */

    /*
    if (event.key == Qt.Key_A) {
        if (event.modifiers & Qt.ControlModifier) {

        }
    }
    */

    /*
    if (event.key == Qt.Key_L) {
        if (event.modifiers & Qt.ControlModifier) {
            // TODO: Go to page dialog
        }
    }
    */

    /*
    if (event.key == Qt.Key_Question) {
        if (event.modifiers & (Qt.ControlModifier | Qt.ShiftModifier)) {
            // TODO: Keyboard shortcuts
        }
    }
    */
}
