/*
  Copyright (C) 2014, 2015 Stefano Verzegnassi <stefano92.100@gmail.com>

    This is free software: you can redistribute it and/or modify
  it under the terms of the  GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License.

    This is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
  along with this program. If not, see http://www.gnu.org/licenses/.
*/

#ifndef FSWATCHER_H
#define FSWATCHER_H

#include <QFileSystemWatcher>
#include <QStringList>

class FSWatcher : public QFileSystemWatcher
{
    Q_OBJECT

public:
    FSWatcher(QObject *parent = 0);

    void addDirectory(const QString &path);
    void clear();

Q_SIGNALS:
    void fileRemoved(const QString & path);
    void fileAdded(const QString & path);
    void fileModified(const QString & path);

    void directoryAdded(const QString & path);
    void directoryRemoved(const QString & path);

private Q_SLOTS:
    void q_fileChanged(const QString & path);
    void q_dirChanged(const QString & path);

private:
    void parseDirectoryContent(QString path);

    // Used for tracking removed files when a folder is removed.
    QStringList m_cachedFilesList;
};

#endif // FSWATCHER_H
