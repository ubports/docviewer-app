/*
 * Copyright: 2015 Canonical Ltd.
 *
 * This file is part of docviewer
 *
 * docviewer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * reminders is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. 
 */

// Uncomment if you need to use QML analyzer
// #define QT_QML_DEBUG

#include "docviewer-application.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName("com.ubuntu.docviewer");
    QCoreApplication::setOrganizationDomain("com.ubuntu.docviewer");

    DocViewerApplication app(argc, argv);
    if (!app.init())
        return 0;

    app.exec();
}
