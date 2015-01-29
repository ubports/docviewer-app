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

#ifndef FILEREADER_H
#define FILEREADER_H

#include <QObject>
#include <QString>

class FileReader: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString path READ getPath WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(QString fileString READ getfileString NOTIFY fileStringChanged)
    Q_PROPERTY(int error READ getError NOTIFY errorChanged)

public:
    explicit FileReader(QObject *parent = 0);
    ~FileReader();

protected:
    QString getPath() { return path; }
    void setPath(QString p);

    QString getfileString() { return fileString; }

    int getError() { return error; }

    QString path;
    QString fileString;
    int error;

private:
    void readString();

Q_SIGNALS:
    void pathChanged();
    void fileStringChanged();
    void errorChanged();
};

#endif // FILEREADER_H
