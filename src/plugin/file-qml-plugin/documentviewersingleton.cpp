/*
  Copyright (C) 2015 Canonical, Ltd.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License 3 as published by
  the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see http://www.gnu.org/licenses/.
*/

#include "documentviewersingleton.h"

#include <QFileInfo>
#include <QDir>
#include <QMimeDatabase>
#include <QStandardPaths>

bool DocumentViewerSingleton::exists(const QString &path)
{
    QFileInfo fi(path);

    if (fi.isFile())
        return fi.exists();

    // else
    return QDir(path).exists();
}

bool DocumentViewerSingleton::copy(const QString &source, const QString &destination)
{
    return QFile::copy(source, destination);
}

bool DocumentViewerSingleton::isFileSupported(const QString &path)
{
    QMimeDatabase mdb;
    const QString mimetype = mdb.mimeTypeForFile(path).name();

    return mimetype.startsWith("text/")
            || mimetype == "application/pdf"
            || mimetype.startsWith("application/vnd.oasis.opendocument")
            || mimetype == "application/msword"
            || mimetype == "application/vnd.openxmlformats-officedocument.wordprocessingml.document"
            || mimetype == "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"
            || mimetype == "application/vnd.openxmlformats-officedocument.presentationml.presentation"
            || mimetype == "application/vnd.ms-excel"
            || mimetype == "application/vnd.ms-powerpoint";
}

QString DocumentViewerSingleton::getXdgDocumentsLocation()
{
    return QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
}

QString DocumentViewerSingleton::buildDestinationPath(const QString &destinationDir, const QString &sourcePath)
{
    QFileInfo fi(sourcePath);

    /*
      We don't support formats that use a double extension
      (e.g. tar.gz), so we can safely use completeBaseName() and
      suffix() functions, in order to properly detect the name of
      the document even when there's a dot in the middle of the name.
    */
    QString suffix = fi.suffix();
    QString filenameWithoutSuffix = fi.completeBaseName();

    QMimeDatabase mdb;
    QMimeType mt = mdb.mimeTypeForFile(sourcePath);

    // If the filename doesn't have an extension add one from the
    // detected mimetype
    if (suffix.isEmpty())
        suffix = mt.preferredSuffix();

    QString destination = QString("%1.%2").arg(
                destinationDir + QDir::separator() + filenameWithoutSuffix,
                suffix);

    // If there's already a file of this name, reformat it to
    // "filename (copy x).png" where x is a number, incremented until we find an
    // available filename.
    if (QFile::exists(destination)) {
        /*
         TRANSLATORS: This string is used for renaming a copied file,
         when a file with the same name already exists in user's
         Documents folder.

         e.g. "Manual_Aquaris_E4.5_ubuntu_EN.pdf" will become
              "Manual_Aquaris_E4.5_ubuntu_EN (copy 2).pdf"

              where "2" is given by the argument "%1"
        */
        QString reformattedSuffix = QString(tr("copy %1"));

        // Check if the file has already a "copy" suffix
        // If so, remove it since we will update it later.
        QRegExp rx(" \\(" + reformattedSuffix.arg(QString("\\d+")) + "\\)");

        if (filenameWithoutSuffix.lastIndexOf(rx) != -1)
            filenameWithoutSuffix.truncate(reformattedSuffixPos);

        // Add the right "copy" suffix.
        int append = 1;
        while (QFile::exists(destination)) {
            destination = QString("%1 (%2).%3").arg(
                        dir + filenameWithoutSuffix,
                        reformattedSuffix.arg(QString::number(append)),
                        suffix);
            append++;
        }
    }

    return destination;
}
