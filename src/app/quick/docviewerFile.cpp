/*
 * Copyright (C) 2013-2014 Canonical, Ltd.
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

#include <QObject>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QMimeDatabase>

#include "docviewerFile.h"

/*
 ----8<-----

 import org.docviewer.file 1.0

 Rectangle {
   width: 200
   height: 200

   File {
      id: file
      path: "the/path/of/file"

      onMimetypeChanged: {
        do.something(mimetype);
      }
   }

   Text {
     anchors.centerIn: parent
     text: helloType.helloworld
   }
 }

 -----8<------
*/

DocviewerFile::DocviewerFile(QObject *parent) :
    QObject(parent),
    path("")
{
}
DocviewerFile::~DocviewerFile() {
    
}

void DocviewerFile::setPath(QString p) {
    if (p.isEmpty()) {
        this->path = "";
    }
    else {
        this->path = QFileInfo(QDir::currentPath(), p).absoluteFilePath();
    }

    qDebug() << "[FILE] Path parsed as:" << this->path;

    this->open();

    emit pathChanged();
}

void DocviewerFile::open() {
    if (!path.isEmpty())
    {
        QFileInfo file(path);

        if (file.exists()) {
            qDebug() << "[FILE] Extracting information from the file...";

            /**Get info of the file**/
            size = file.size();
            emit sizeChanged();

            lastmodified = file.lastModified();
            emit lastmodifiedChanged();

            creationTime = file.created();
            emit creationTimeChanged();

            /**Get mimetype**/
            this->readMimeType();
        }
        else {
            qDebug() << "[FILE] ERROR: Requested file does not exist!";
            error = -1;
            emit errorChanged();
        }
    }
}

void DocviewerFile::readMimeType()
{
    QMimeDatabase db;
    mimetype = db.mimeTypeForFile(this->path).name();
    description = db.mimeTypeForFile(this->path).comment();

    if (mimetype == "application/octet-stream") {
        // Returned by Qt when it cannot determinate the mime type
        mimetype = "Unknown";
    }

    qDebug() << "[FILE] Requested file mime type:" << mimetype;
    qDebug() << "[FILE] Requested file description:" << description;
    emit mimetypeChanged();
    emit descriptionChanged();
}
