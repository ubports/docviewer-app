/*
 *  Copyright (C) 2014, 2015
 *                  Stefano Verzegnassi <stefano92.100@gmail.com>
 *
 *  This is free software: you can redistribute it and/or modify
 *  it under the terms of the  GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License.
 *
 *  This is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see http://www.gnu.org/licenses/.
 */

#include "fswatcher.h"
#include <QDirIterator>

#include <QFile>
#include <QDir>

#include <QDebug>

FSWatcher::FSWatcher(QObject *parent)
    : QFileSystemWatcher(parent)
{
    connect(this, SIGNAL(fileChanged(QString)), this, SLOT(q_fileChanged(QString)));
    connect(this, SIGNAL(directoryChanged(QString)), this, SLOT(q_dirChanged(QString)));
}

void FSWatcher::clear()
{
    if (!this->directories().isEmpty())
        this->removePaths(this->directories());

    if (!this->files().isEmpty())
        this->removePaths(this->files());
}

void FSWatcher::addDirectory(const QString &path)
{
    if (!this->directories().contains(path))
    {
        this->addPath(path);
        Q_EMIT directoryAdded(path);

        parseDirectoryContent(path);
    }
}

void FSWatcher::q_fileChanged(const QString &path)
{
    // Check if file has been removed
    if (!QFile::exists(path))
    {
        Q_EMIT fileRemoved(path);
        return;
    }

    /* We got a signal for a file already watched, but it's not been deleted.
       That means that it has been modified. */
    Q_EMIT fileModified(path);
}

void FSWatcher::q_dirChanged(const QString &path)
{
    QDir dir(path);

    /* Check if file has been removed.
       When this happens, files in that folder are automatically removed
       from watcher. We need to manually notify their removal.
       */
    if (!dir.exists())
    {
        Q_FOREACH (const QString& file, m_cachedFilesList)
        {
            if (!this->files().contains(file))
            {
                Q_EMIT fileRemoved(file);
              //  qDebug() << "file removed" << file;
            }
        }

        m_cachedFilesList = this->files();

        Q_EMIT directoryRemoved(path);
        return;
    }

    // The content of the directory has changed (a file has been added or removed)
    // Let's check it out!
    parseDirectoryContent(path);
}

void FSWatcher::parseDirectoryContent(QString path)
{
    QString filePath;
    QDirIterator dir(path, QDir::Files | QDir::NoDotAndDotDot | QDir::Readable,
                     QDirIterator::Subdirectories);

    while (dir.hasNext())
    {
        dir.next();
        filePath = dir.filePath();

        // Do not check for files already watched.
        if (!this->files().contains(filePath))
        {
            // Add the file to the tracker
            this->addPath(filePath);
            Q_EMIT fileAdded(filePath);
        }
    }

    m_cachedFilesList = this->files();
}
