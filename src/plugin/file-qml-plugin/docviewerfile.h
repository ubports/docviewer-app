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

#ifndef DOCVIEWERFILE_H
#define DOCVIEWERFILE_H

#include <QObject>
#include <QVariantMap>

class DocviewerFile : public QObject
{
    Q_OBJECT
    Q_PROPERTY( QString path READ getPath WRITE setPath NOTIFY pathChanged )
    Q_PROPERTY( QVariantMap mimetype READ getMimetype NOTIFY mimetypeChanged)
    Q_PROPERTY( QVariantMap info READ getInfo NOTIFY infoChanged)
    Q_PROPERTY( int error READ getError NOTIFY errorChanged )
    
public:
    explicit DocviewerFile(QObject *parent = 0);
    ~DocviewerFile();
    
protected:
    QString getPath() { return m_path; }
    void setPath(const QString &path);

    QVariantMap getMimetype() { return m_mimetype; }
    QVariantMap getInfo() { return m_info; }
    int getError() { return m_error; }
    
Q_SIGNALS:
    void pathChanged();
    void mimetypeChanged();
    void errorChanged();
    void infoChanged();

private slots:
    void open();

private:
    void setMimetype();

    QString m_path;
    QVariantMap m_mimetype;
    QVariantMap m_info;
    int m_error;
};

#endif // DOCVIEWERFILE_H
