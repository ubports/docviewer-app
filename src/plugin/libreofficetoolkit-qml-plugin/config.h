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

#ifndef CONFIG_H
#define CONFIG_H

// FIXME: Perhaps we want to use smaller tiles on mobile devices?
#define TILE_SIZE 256.0

// Uncomment it if you want to see tiles boundaries
//#define DEBUG_SHOW_TILE_BORDER

// Uncomment for benchmarking tile rendering performance
//#define DEBUG_TILE_BENCHMARK

// Uncomment if you want more verbose application output
//#define DEBUG_VERBOSE

#include <QDir>
#include <QDebug>

inline static const char* getLibreOfficePath() {
    // LibreOffice installation path on Debian/Ubuntu
    const char* path = "/usr/lib/librefice/program";

    if (QDir(QString::fromUtf8(path)).exists()) {
        qDebug() << "LibreOffice binaries found at:" << path;
        return path;
    }

    const char* loClickPath = "libreoffice/program";

    char* libPaths = getenv("LD_LIBRARY_PATH");
    char* libPathsArray = strtok(libPaths, ":");

    while (libPathsArray) {
        QDir clickDir(QString::fromUtf8(libPathsArray));

        if (clickDir.cd(QString::fromUtf8(loClickPath))) {
            char* clickPath = (char*) malloc(1 + strlen(libPathsArray) + 1 + strlen(loClickPath));

            strcpy(clickPath, libPathsArray);
            strcat(clickPath, "/");
            strcat(clickPath, loClickPath);

            qDebug() << "LibreOffice binaries found at:" << clickPath;
            return clickPath;
        }
    }

    qDebug() << "LibreOffice binaries not found.";
    return NULL;
}

#endif // CONFIG_H
