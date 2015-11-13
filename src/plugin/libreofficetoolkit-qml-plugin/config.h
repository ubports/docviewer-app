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

#define TILE_SIZE 256.0

// Uncomment it if you want to see tiles boundaries
#define DEBUG_SHOW_TILE_BORDER

// Uncomment for benchmarking tile rendering performance
//#define DEBUG_TILE_BENCHMARK

// Uncomment if you want more verbose application output
#define DEBUG_VERBOSE

#include <QDir>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QDebug>

class Config
{

public:
    static const char* getLibreOfficePath() {
        QString loPath;

        // LibreOffice installation path on Debian/Ubuntu
        QString path = "/usr/lib/libreoffice/program";

        if (QDir(path).exists()) {
            loPath = path;
        }

        // Check if LibreOffice has been provided through a .click package
        else {
            QString libPaths = getenv("LD_LIBRARY_PATH");

            Q_FOREACH(QString libPath, libPaths.split(":")) {
                QDir clickDir(libPath);

                if (clickDir.cd("libreoffice/program")) {
                    QString clickPath = libPath + "/libreoffice/program";

                    loPath = clickPath;
                }
            }
        }

        if (loPath.isEmpty()) {
            qDebug() << "LibreOffice binaries not found.";

            return NULL;
        }

        else {
            char *data = new char[loPath.toLatin1().size() + 1];
            strcpy(data, loPath.toLatin1().data());

            qDebug() << "LibreOffice binaries found at:" << loPath;

            return data;
        }
    }

    static const char* getLibreOfficeProfilePath() {
        QString configLocation = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
        QString appPkgName = QCoreApplication::organizationDomain();

        QString profilePath = "file://" + configLocation + "/" + appPkgName + "/libreoffice/4";

        qDebug() << "LibreOffice profile path:" << profilePath;

        char *data = new char[profilePath.toLatin1().size() + 1];
        strcpy(data, profilePath.toLatin1().data());

        return data;
    }
};

#endif // CONFIG_H
