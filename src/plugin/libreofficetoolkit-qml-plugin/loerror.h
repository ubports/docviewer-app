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
 */

#ifndef LOERROR_H
#define LOERROR_H

#include <QObject>

class LibreOfficeError : public QObject
{
    Q_OBJECT
    Q_ENUMS(Error)

public:
    enum Error {
        NoError                   = 0,
        LibreOfficeNotFound       = 1,
        LibreOfficeNotInitialized = 2,
        DocumentNotLoaded         = 3
    };
};

#endif // LOERROR_H
