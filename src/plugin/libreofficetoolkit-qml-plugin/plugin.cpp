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

#include <QtQml>
#include <QtQml/QQmlContext>

#include "plugin.h"
#include "lodocument.h"
#include "loview.h"

void LOPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("DocumentViewer.LibreOffice"));
    
    //@uri DocumentViewer.LibreOffice
    qmlRegisterType<LODocument>(uri, 1, 0, "Document");
    qmlRegisterType<LOView>(uri, 1, 0, "View");
}

void LOPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    QQmlExtensionPlugin::initializeEngine(engine, uri);
}
