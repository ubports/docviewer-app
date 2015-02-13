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

#include "urlhandler.h"

#include <QUrl>
#include <QStringList>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

UrlHandler::UrlHandler()
    : m_documentFile("")
{
    m_validSchemes << "document";
}

/*!
 * @brief UrlHandler::processUri parsers our input uri and sets attributes accordingly.
 * @param QString uri to parse and set attributes.
 * @return false if invalid parameter is input.
 */
bool UrlHandler::processUri(const QString& arg)
{
    QUrl uri(arg);

    if (!m_validSchemes.contains(uri.scheme())) {
        return false;
    }

    if (uri.scheme() == "document") {
        uri.setScheme("file");
    }

    if (uri.isRelative()) {
        uri = QUrl::fromLocalFile(QDir::current().absoluteFilePath(arg));
    }

    // Check if it's a local file
    if (uri.isValid() && uri.isLocalFile()) {
        QFileInfo info(uri.toLocalFile());
        if (info.exists() && info.isFile()) {
            m_documentFile = info.absoluteFilePath();
            return true;
        } else {
            qWarning() << "File not found:" << uri << info.exists() << info.isFile();
        }
    } else {
        qWarning() << "Invalid uri:" << uri;
    }

    return false;
}
