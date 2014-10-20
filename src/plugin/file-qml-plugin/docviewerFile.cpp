#include <QProcess>
#include <QObject>
#include <QDebug>
#include <QFileInfo>
#include <QDir>

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

    this->open();

    emit pathChanged();
}

void DocviewerFile::open() {
    if (!path.isEmpty())
    {
        QFileInfo file(path);

        if (file.exists()) {

            /**Get info of the file**/
            size = file.size();
            emit sizeChanged();

            lastmodified = file.lastModified();
            emit lastmodifiedChanged();

            /**Get mimetype**/
            QStringList mimeTypeCommand;
            mimeTypeCommand << "-ib" << path;

            mimeTypeProcess = new QProcess();

            this->connect(mimeTypeProcess, SIGNAL(readyReadStandardOutput()), SLOT(s_readMimeType()));
            this->connect(mimeTypeProcess, SIGNAL(finished(int)), SLOT(s_finished(int)));

            mimeTypeProcess->start("file", mimeTypeCommand);

        }
        else {
            error = -1;
            emit errorChanged();
        }
    }


}

/****SLOT****/

void DocviewerFile::s_readMimeType()
{
    mimetype = mimeTypeProcess->readAllStandardOutput();
    mimetype = mimetype.left(mimetype.size()-1);
}

void DocviewerFile::s_finished(int exitCode)
{
    if (exitCode != 0)
        mimetype = "Unknown";
    emit mimetypeChanged();
}
