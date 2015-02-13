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

#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

#include <QHash>
#include <QSize>
#include <QString>

class UrlHandler;

/*!
 * @brief The CommandLineParser is used to parse our commandline inputs and set
 * parameters accordingly.
 */
class CommandLineParser
{
public:
    CommandLineParser();

    bool processArguments(const QStringList& args);

    bool pickModeEnabled() const { return m_pickMode; }
    bool testability() const { return m_testability; }
    const QString &documentFile() const { return m_documentFile; }

private:
    bool invalidArg(QString arg);
    void usage();

    UrlHandler *m_urlHandler;

    bool m_pickMode;
    bool m_testability;
    QString m_documentFile;
};

#endif // COMMANDLINEPARSER_H
