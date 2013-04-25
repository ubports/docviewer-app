#include <QProcess>
#include <QObject>
#include <QDebug>
#include <QFileInfo>

#include "file.h"

File::File(QString *_path)
{
    this->path = *_path;
    mimeType = new QString();
    error = new QString();

    if (!path.isEmpty())
    {
        /**Get mimetype**/
        QStringList mimeTypeCommand;
        mimeTypeCommand << "-ib" << path;

        mimeTypeProcess = new QProcess();

        this->connect(mimeTypeProcess, SIGNAL(readyReadStandardOutput()), SLOT(s_readMimeType()));
        this->connect(mimeTypeProcess, SIGNAL(finished(int)), SLOT(s_finished(int)));

        mimeTypeProcess->start("file", mimeTypeCommand);

        /**Get size of file**/
        QFileInfo file(path);

        size = file.size();
        lastModified = file.lastModified();
    }
}

void File::setError(QString _error)
{
    *this->error = _error;

    emit errorDetected();
}

/****SLOT****/

void File::s_readMimeType()
{
    *mimeType = mimeTypeProcess->readAllStandardOutput();
    *mimeType = mimeType->left(mimeType->size()-1);
}

void File::s_finished(int exitCode)
{
    if (exitCode != 0)
        *mimeType = "Unknow";
    emit mimetypeChanged();
}

/****GETTER****/

QString File::getPath()
{
    return path;
}

QString File::getMimetype()
{
    if (mimeTypeProcess->state() == QProcess::Starting || mimeTypeProcess->state() == QProcess::Running)
        return "Loading...";
    else
        return *mimeType;
}

qint64 File::getSize()
{
    return size;
}

QDateTime File::getLastModified()
{
    return lastModified;
}

QString File::getError()
{
    return *error;
}
