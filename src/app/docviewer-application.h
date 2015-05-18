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
    Q_PROPERTY(bool pickModeEnabled READ pickModeEnabled NOTIFY pickModeEnabledChanged)
    Q_PROPERTY(bool desktopMode READ isDesktopMode CONSTANT)
    Q_PROPERTY(bool fullScreen READ isFullScreen WRITE setFullScreen NOTIFY fullScreenChanged)
    Q_PROPERTY(QString documentFile READ getDocumentFile WRITE setDocumentFile NOTIFY documentFileChanged)
    Q_PROPERTY(QString documentsDir READ getDocumentsDir CONSTANT)

public:
    enum UiMode{
        BrowseContentMode,
        PickContentMode
    };

    explicit DocViewerApplication(int& argc, char** argv);
    virtual ~DocViewerApplication();

    bool init();
    int exec();

    void setDefaultUiMode(UiMode mode);
    UiMode defaultUiMode() const;
    void setUiMode(UiMode mode);
    bool pickModeEnabled() const;
    bool isDesktopMode() const;
    bool isFullScreen() const;
    const QString &getDocumentFile() const;
    const QString &getDocumentsDir() const;

    Q_INVOKABLE void returnPickedContent(QList<QString> paths);
    Q_INVOKABLE void contentPickingCanceled();
    Q_INVOKABLE void parseUri(const QString &arg);
    Q_INVOKABLE void releaseResources();

signals:
    void pickModeEnabledChanged();
    void fullScreenChanged();
    void documentFileChanged();
    void browseModeRequested();

private slots:
    void switchToPickMode();
    void switchToBrowseMode();
    void setFullScreen(bool fullScreen);
    void setDocumentFile(const QString &documentFile);

private:
    void registerHub();
    void registerQML();
    void createView();

    QQuickView *m_view;
    CommandLineParser* m_cmdLineParser;
    UrlHandler *m_urlHandler;
    ContentCommunicator *m_contentCommunicator;

    bool m_pickModeEnabled;
    UiMode m_defaultUiMode;
    QString m_documentFile;
    bool m_documentLoaded;
};

#endif // DOCVIEWERAPPLICATION_H
