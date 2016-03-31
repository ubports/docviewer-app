/*
 * Copyright (C) 2015, 2016 Stefano Verzegnassi
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
 
// Here we handle all the key events that are not
// recognised by UITK ScrollView

function parseEvent(event) {
    var view = loPage.contentItem.loView
    var isPresentation = view.document.documentType === LibreOffice.Document.PresentationDocument

    if (event.key == Qt.Key_PageUp) {
        if (isPresentation) {
            view.currentPart -= 1
            event.accepted = true
        }
        return;
    }
 
    if (event.key == Qt.Key_PageDown) {
        if (isPresentation) {
            view.currentPart += 1
            event.accepted = true
        }
        return;
    }
 
    if (event.key == Qt.Key_Home) {
        if (event.modifiers & Qt.ControlModifier)
            view.currentPart = 0

        event.accepted = false
        return
    }

    if (event.key == Qt.Key_End) {
        if (event.modifiers & Qt.ControlModifier)
            view.currentPart = view.document.partsCount - 1

        event.accepted = false
        return
    }

    if (event.key == Qt.Key_Plus) {
        if (event.modifiers & Qt.ControlModifier) {
            view.setZoom(Math.min(view.zoomSettings.maximumZoom, view.zoomSettings.zoomFactor + 0.25))
        }

        return
    }

    if (event.key == Qt.Key_Minus) {
        if (event.modifiers & Qt.ControlModifier) {
            view.setZoom(Math.max(view.zoomSettings.minimumZoom, view.zoomSettings.zoomFactor - 0.25))
        }

        return
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
