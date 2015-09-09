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

#include <QCoreApplication>
#include <QDir>
#include <QDebug>

inline static const char* getLibreOfficePath() {
    // LibreOffice installation path on Debian/Ubuntu
    QString path = "/usr/lib/libreoffice/program/";

    if (!QDir(path).exists()) {
        // Click packages of docviewer includes LibreOffice by themselves.
        // We know that applicationDirPath() is ./lib/<arch_triplet>/bin
        // FIXME: Hackish!
        QDir clickDir(QCoreApplication::applicationDirPath());
        if (clickDir.cd("../libreoffice/program"))
            path = clickDir.canonicalPath();
        else
            path = "";
    }

    if (!path.isEmpty())
        qDebug() << "LibreOffice binaries found at:" << path;
    else
        qDebug() << "LibreOffice binaries not found.";

    return path.toLocal8Bit().data();
}

#endif // CONFIG_H
