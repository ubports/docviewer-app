/*
 * Copyright (C) 2014 Canonical, Ltd.
 *
 * Authors:
 *  Arthur Mello <arthur.mello@canonical.com>
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

#ifndef URLHANDLER_H
#define URLHANDLER_H

#include <QString>
#include <QList>

/*!
 * @brief The UrlHandler is used to parse calls of docviewer from the url schema.
 */
class UrlHandler
{
public:
    UrlHandler();

    bool processUri(const QString &arg);
    const QString &documentFile() { return m_documentFile; }

private:
    QList<QString> m_validSchemes;
    QString m_documentFile;
};

#endif // URLHANDLER_H
