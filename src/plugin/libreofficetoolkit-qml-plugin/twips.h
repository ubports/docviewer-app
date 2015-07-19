/*
 * Copyright (C) 2015 Canonical, Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3, as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Stefano Verzegnassi <stefano92.100@gmail.com>
 */

#ifndef TWIPS_H
#define TWIPS_H

// FIXME: Should we check for the real DPI of the screen, since we'll have in
// future to support HiDPI devices and Full/Ultra-HD smartphones/tablet?
// FIXME: Not common, but DPI on the X axis, and DPI on the Y axis may be
// different.
#define VIRTUAL_DPI 96.0

#include <QtGlobal>

class Twips
{
public:
    static inline int convertTwipsToPixels(int twips, qreal zoom = 1.0) {
        return int(twips / 1440.0 * VIRTUAL_DPI * zoom);
    }

    static inline int convertPixelsToTwips(int pixels, qreal zoom = 1.0) {
        return int(pixels * 1440.0 / VIRTUAL_DPI / zoom);
    }
};

#endif // TWIPS_H
