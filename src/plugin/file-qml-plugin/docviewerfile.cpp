/*
 * Copyright (C) 2013-2015 Canonical, Ltd.
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

#include "docviewerfile.h"

#include <QDir>
#include <QFileInfo>
#include <QMimeDatabase>
#include <QDateTime>

#include <QDebug>

/*
 ----8<-----

 import DocumentViewer 1.0

 Rectangle {
   width: 200
   height: 200

   File {
      id: file
      path: "the/path/of/file"

      onMimetypeChanged: {
        do.something(mimetype.name);
      }
   }

   Text {
     anchors.centerIn: parent
     text: mimetype.description
   }
 }

 -----8<------
*/

DocviewerFile::DocviewerFile(QObject *parent) :
    QObject(parent),
    m_path("")
{
    connect(this, SIGNAL(pathChanged()), this, SLOT(open()));
}

DocviewerFile::~DocviewerFile()
{
    //
}

void DocviewerFile::setPath(const QString &path)
{
    if (m_path == path)
        return;

    QFileInfo file(QDir::currentPath(), path);

    m_path = file.absoluteFilePath();
    Q_EMIT pathChanged();

    qDebug() << "[FILE] Path parsed as:" << m_path;
}

void DocviewerFile::open()
{
    if (m_path.isEmpty())
        return;

    QFileInfo file(m_path);

    if (file.exists()) {
        qDebug() << "[FILE] Extracting information from the file...";

        /** Get info of the file **/
        m_info["size"] = file.size();
        m_info["lastModified"] = file.lastModified();
        m_info["creationTime"] = file.created();
        Q_EMIT infoChanged();

        /** Get mimetype **/
        this->setMimetype();
    }

    else {
        qDebug() << "[FILE] ERROR: Requested file does not exist!";
        m_error = -1;
        Q_EMIT errorChanged();
    }
}

void DocviewerFile::setMimetype()
{
    QMimeType mime = QMimeDatabase().mimeTypeForFile(m_path);

    m_mimetype["name"] = mime.name();
    m_mimetype["description"] = mime.comment();

    // Use a more user-friendly value for 'name' when Qt can not determinate the
    // mime type.
    if (m_mimetype.value("name") == "application/octet-stream")
        m_mimetype["name"] = "Unknown";

    qDebug() << "[FILE] Requested file mime type:" << m_mimetype.value("name");
    Q_EMIT mimetypeChanged();
}
