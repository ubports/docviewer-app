/*
 * Copyright (C) 2013 Canonical, Ltd.
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

#include <QFile>
#include <QObject>
#include <QDebug>
#include <QTextStream>

#include "fileReader.h"

/*
 ----8<-----

 import org.docviewer.file 1.0

 Rectangle {
   width: 200
   height: 200

   FileReader {
      id: readFile
      path: "/path/of/file"
   }

   Text {
     anchors.centerIn: parent
     text: readFile.fileString
   }
 }

 -----8<------
*/

FileReader::FileReader(QObject *parent) :
    QObject(parent),
    path("")
{

}

FileReader::~FileReader() {

}

void FileReader::setPath(QString p) {
    if (p.isEmpty())
    {
        this->path = "";
    }
    else
    {
        this->path = p;
    }

    this->readString();

    emit pathChanged();
}

void FileReader::readString() {
    if (!path.isEmpty()) {

        QFile mfile(path);

        if (!mfile.open(QFile::ReadOnly | QFile::Text))
        {
            qDebug() << "Could not open file for reading";
            error = -1;
            emit errorChanged();
            return;
        }

        QTextStream in(&mfile);
        fileString = in.readAll();
        emit fileStringChanged();

        mfile.flush();
        mfile.close();
    }
    else
    {
        error = -1;
        emit errorChanged();
    }
}
