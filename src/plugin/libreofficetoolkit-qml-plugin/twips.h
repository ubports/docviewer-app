/*
 * Copyright (C) 2015, 2016 Stefano Verzegnassi
 * Copyright (C) 2015 Roman Shchekin
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
 */

#ifndef TWIPS_H
#define TWIPS_H

#include <QtGlobal>
#include <QtWidgets/QApplication>
#include <QScreen>

#include "ucunits.h"

#define DEFAULT_DPI 96
#define DEFAULT_GRID_UNIT_PX 8

class Twips
{
public:
    enum ConvertionMode {
        UseDPI = 0,
        UseUnits = 1
    };

    static inline int convertTwipsToPixels(int twips, qreal zoom = 1.0,
                                           ConvertionMode mode = ConvertionMode::UseUnits)
    {
        if (mode == ConvertionMode::UseUnits) {
            qreal ratio = getUnitsRatio();
            return int(twips / 1440.0 * (DEFAULT_DPI * ratio) * zoom);
        } else {
            qreal dpi = getLogicalDotsPerInch();
            return int(twips / 1440.0 * (dpi ? dpi : DEFAULT_DPI) * zoom);
        }
    }

    static inline int convertPixelsToTwips(int pixels, qreal zoom = 1.0,
                                           ConvertionMode mode = ConvertionMode::UseUnits)
    {
        if (mode == ConvertionMode::UseUnits) {
            qreal ratio = getUnitsRatio();
            return int(pixels * 1440.0 / (DEFAULT_DPI * ratio) / zoom);
        } else {
            qreal dpi = getLogicalDotsPerInch();
            return int(pixels * 1440.0 / (dpi ? dpi : DEFAULT_DPI) / zoom);
        }
    }

    static inline qreal getLogicalDotsPerInch()
    {
        static qreal value = 0;
        if (!value) {
            QList<QScreen*> screens = QGuiApplication::screens();
            if (screens.size()) {
                QScreen *screen = screens.at(0);
                // Subscribe for changing signal (just to make caching rock-solid).
                QObject::connect(screen, &QScreen::logicalDotsPerInchChanged,
                                 [] (const qreal newValue) { value = newValue; } );
                value = screen->logicalDotsPerInch();
            }
        }

        return value;
    }

    static inline qreal getUnitsRatio()
    {
        return UCUnits::instance().gridUnit() / DEFAULT_GRID_UNIT_PX;
    }
};

#endif // TWIPS_H
