/*
 * Copyright (C) 2013 Canonical, Ltd.
 *
 * Authors:
 *  Nicolas d'Offay <nicolas.doffay@canonical.com>
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
 *
 */

#include "command-line-parser.h"

#include "urlhandler.h"

#include <QDebug>
#include <QDir>
#include <QStandardPaths>
#include <QTextStream>
#include <QUrl>

CommandLineParser::CommandLineParser()
    : m_pickMode(false),
      m_testability(false),
      m_isFullscreen(false),
      m_documentFile(""),
      m_documentsDir("")
{
    m_urlHandler = new UrlHandler();
}

/*!
 * @brief CommandLineParser::processArguments parsers our input commandline args and sets attributes accordingly.
 * @param QStringList of commandline args to parse and set attributes.
 * @return false if invalid parameter is input or -h/--help is called.
 */
bool CommandLineParser::processArguments(const QStringList& args)
{
    bool valid_args = true;

    for (int i = 1; i < args.count(); ++i)
    {
        if (args[i] == "--help" || args[i] == "-h") {
            usage();
            return false;
        }
        else if (args[i] == "--fullscreen") {
            m_isFullscreen = true;
        }
        else if (args[i] == "--pick-mode") {
            m_pickMode = true;
        }
        else if (args[i] == "--testability") {
            m_testability = true;
        }
        else if (args[i].contains("--documents-dir")) {
            // Extract the given path
            QString dirPath = args[i].split("--documents-dir=").last();

            if (!dirPath.isEmpty()) {
                QDir di(dirPath);

                if (di.exists())
                    m_documentsDir = di.absolutePath();
                else {
                    QTextStream(stderr) << m_documentsDir << ": Not found" << endl;
                    valid_args = false;
                }

                i++;
            }
            else {
                QTextStream(stderr) << "Missing PATH argument for --documents-dir'" << endl;
                usage();
                valid_args = false;
            }
        }
        else {
            if (args[i].startsWith("--desktop_file_hint")) {
                // ignore this command line switch, hybris uses it to get application info
            }
            else if (!args.at(i).isEmpty()) {
                QFileInfo fi(args.at(i));

                if (fi.exists()) {
                    m_documentFile = fi.absoluteFilePath();
                }
                else if (m_urlHandler->processUri(args.at(i))) {
                    m_documentFile = m_urlHandler->documentFile();
                }
            }
            else {
                valid_args = !invalidArg(args[i]);
            }
        }
    }

    return valid_args;
}

/*!
 * @brief CommandLineParser::usage() prints out our form factors.
 */
void CommandLineParser::usage()
{
    QTextStream out(stdout);
    out << "Usage: ubuntu-docviewer-app [options] [file_path]" << endl;
    out << "Options:" << endl;
    out << "  --fullscreen\trun fullscreen" << endl;
    out << "  --pick-mode\t\tEnable mode to pick photos" << endl;
    out << "  file_path\t\tOpens ubuntu-docviewer-app displaying the selected file" << endl;
    out << "  --documents-dir=PATH\n\t\tLoad the list of documents from the given folder, instead of default ~/Documents.\nThe path must exist prior to running ubuntu-docviewer-app" << endl;
}

/*!
 * @brief CommandLineParser::invalidArg() if an invalid argument is contained in our QStringList.
 * @return returns true.
 */
bool CommandLineParser::invalidArg(QString arg)
{
    QTextStream(stderr) << "Invalid argument '" << arg << "'" << endl;
    usage();

    return true;
}
