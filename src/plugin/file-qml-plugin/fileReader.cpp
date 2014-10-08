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
