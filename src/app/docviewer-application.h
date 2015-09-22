/*
 * Copyright (C) 2012 Canonical Ltd
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors:
 * Charles Lindsay <chaz@yorba.org>
 */

#ifndef DOCVIEWERAPPLICATION_H
#define DOCVIEWERAPPLICATION_H

#include <QApplication>
#include <QElapsedTimer>
#include <QFileInfo>
#include <QTimer>

class CommandLineParser;
class UrlHandler;
class ContentCommunicator;

class QQuickView;

/*!
 * \brief The DocViewerApplication class
 */
class DocViewerApplication : public QApplication
{
    Q_OBJECT
    Q_PROPERTY(bool desktopMode READ isDesktopMode CONSTANT)
    Q_PROPERTY(bool fullScreen READ isFullScreen WRITE setFullScreen NOTIFY fullScreenChanged)
    Q_PROPERTY(QString documentFile READ getDocumentFile WRITE setDocumentFile NOTIFY documentFileChanged)
    Q_PROPERTY(QString documentsDir READ getDocumentsDir CONSTANT)

public:
    explicit DocViewerApplication(int& argc, char** argv);
    virtual ~DocViewerApplication();

    bool init();
    int exec();

    bool isDesktopMode() const;
    bool isFullScreen() const;
    const QString &getDocumentFile() const;
    const QString &getDocumentsDir() const;

    Q_INVOKABLE void parseUri(const QString &arg);
    Q_INVOKABLE void releaseResources();

signals:
    void fullScreenChanged();
    void documentFileChanged();

private slots:
    void setFullScreen(bool fullScreen);
    void setDocumentFile(const QString &documentFile);

private:
    void registerQML();
    void createView();

    QQuickView *m_view;
    CommandLineParser* m_cmdLineParser;
    UrlHandler *m_urlHandler;

    QString m_documentFile;
    bool m_documentLoaded;
};

#endif // DOCVIEWERAPPLICATION_H
