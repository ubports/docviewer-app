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

#include "docviewer-application.h"
#include "command-line-parser.h"
#include "urlhandler.h"

#include <QQuickItem>
#include <QStandardPaths>
#include <QQuickView>
#include <QtQml/QtQml>
#include <QString>
#include <QUrl>
#include <QtGui/QGuiApplication>

/*!
 * \brief DocViewerApplication::DocViewerApplication
 * \param argc
 * \param argv
 */
DocViewerApplication::DocViewerApplication(int& argc, char** argv)
    : QApplication(argc, argv),
      m_view(new QQuickView()),
      m_documentFile(""),
      m_documentLoaded(false)
{
   //
}

bool DocViewerApplication::init()
{
    m_cmdLineParser = new CommandLineParser();

    if (!m_cmdLineParser->processArguments(arguments()))
        return false;

    if (qgetenv("QT_LOAD_TESTABILITY") == "1" || m_cmdLineParser->testability()) {
        QLibrary testLib(QLatin1String("qttestability"));
        if (testLib.load()) {
            typedef void (*TasInitialize)(void);
            TasInitialize initFunction = (TasInitialize)testLib.resolve("qt_testability_init");
            if (initFunction) {
                initFunction();
            } else {
                qCritical("Library qttestability resolve failed!");
            }
        } else {
            qCritical("Library qttestability load failed!");
        }
    }

    m_urlHandler = new UrlHandler();

    registerQML();

    // FIXME: Broken after removal of it.
    /*if (m_cmdLineParser->pickModeEnabled())
        setDefaultUiMode(DocViewerApplication::PickContentMode);
*/
    return true;
}

/*!
 * \brief DocViewerApplication::~DocViewerApplication
 */
DocViewerApplication::~DocViewerApplication()
{
    delete m_view;
    delete m_cmdLineParser;
}

/*!
 * \brief DocViewerApplication::exec
 * \return
 */
int DocViewerApplication::exec()
{
    createView();

    return QApplication::exec();
}

/*!
 * \brief DocViewerApplication::registerQML
 */
void DocViewerApplication::registerQML()
{
    // Set up import paths
    QStringList importPathList = m_view->engine()->importPathList();

    // Prepend the location of the plugin in the build dir,
    // so that Qt Creator finds it there, thus overriding the one installed
    // in the sistem if there is one
    importPathList.prepend(QCoreApplication::applicationDirPath() + "/../plugin/");
    m_view->engine()->setImportPathList(importPathList);
}

/*!
 * \brief DocViewerApplication::isDesktopMode
 * Returns true if the DESKTOP_MODE env var is set
 */
bool DocViewerApplication::isDesktopMode() const
{

  // Assume that platformName (QtUbuntu) with ubuntu
  // in name means it's running on device
  // TODO: replace this check with SDK call for formfactor
  QString platform = QGuiApplication::platformName();
  return !((platform == "ubuntu") || (platform == "ubuntumirclient"));
}

/*!
 * \brief DocViewerApplication::isFullScreen
 * Returns true if window is on FullScreen mode
 */
bool DocViewerApplication::isFullScreen() const
{
    return m_view->windowState() == Qt::WindowFullScreen;
}

/*!
 * \brief DocViewerApplication::getDocumentFile
 * Returns the document file passed as a parameter
 */
const QString& DocViewerApplication::getDocumentFile() const
{
    return m_documentFile;
}

/*!
 * \brief DocViewerApplication::getDocumentsDir
 * Returns the documents dir passed as a parameter
 */
const QString& DocViewerApplication::getDocumentsDir() const
{
    return m_cmdLineParser->documentsDir();
}


/*!
 * \brief DocViewerApplication::createView
 * Create the master QDeclarativeView that all the pages will operate within
 */
void DocViewerApplication::createView()
{
    m_view->setTitle(tr("Document Viewer"));

    // Set ourselves up to expose functionality to run external commands from QML...
    m_view->engine()->rootContext()->setContextProperty("DOC_VIEWER", this);

    QObject::connect(m_view->engine(), SIGNAL(quit()), this, SLOT(quit()));

    QString qmlfile;
    const QString filePath = QLatin1String("qml/ubuntu-docviewer-app.qml");
    QStringList paths = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
    paths.prepend(QDir::currentPath());
    paths.prepend(QCoreApplication::applicationDirPath());
    Q_FOREACH (const QString &path, paths) {
        QString myPath = path + QLatin1Char('/') + filePath;

        if (QFile::exists(myPath)) {
            qmlfile = myPath;
            break;
        } else {
            myPath.replace(QCoreApplication::applicationName(), "ubuntu-docviewer-app");

            if (QFile::exists(myPath)) {
                qmlfile = myPath;
                break;
            }
        }
    }
    // sanity check
    if (qmlfile.isEmpty()) {
        qFatal("File: %s does not exist at any of the standard paths!", qPrintable(filePath));
    }

    m_view->setSource(QUrl::fromLocalFile(qmlfile));
    setDocumentFile(m_cmdLineParser->documentFile());

    m_view->setResizeMode(QQuickView::SizeRootObjectToView);

    //run fullscreen if specified at command line
    if (m_cmdLineParser->isFullscreen()) {
        setFullScreen(true);
        m_view->showFullScreen();
    } else {
        m_view->show();
    }
}

/*!
 * \brief DocViewerApplication::setFullScreen
 * Change window state to fullScreen or no state
 */
void DocViewerApplication::setFullScreen(bool fullScreen)
{
    if(fullScreen) {
        m_view->setWindowState(Qt::WindowFullScreen);
    } else {
        m_view->setWindowState(Qt::WindowNoState);
    }

    Q_EMIT fullScreenChanged();
}

void DocViewerApplication::setDocumentFile(const QString &documentFile)
{
    if(!documentFile.isEmpty()) {
        if (m_documentFile != documentFile) {
            m_documentFile = "file://" + documentFile;
            Q_EMIT documentFileChanged();;
        }
    }
}

void DocViewerApplication::parseUri(const QString &arg)
{
    if (m_urlHandler->processUri(arg)) {
        setDocumentFile(m_urlHandler->documentFile());
    }
}

void DocViewerApplication::releaseResources()
{
    if (m_view) {
        m_view->releaseResources();
    }
}
