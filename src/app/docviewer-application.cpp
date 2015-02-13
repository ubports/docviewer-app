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
#include "content-communicator.h"
#include "command-line-parser.h"
#include "urlhandler.h"
#include "quick/documentmodel.h"

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
      m_contentCommunicator(new ContentCommunicator(this)),
      m_pickModeEnabled(false),
      m_defaultUiMode(BrowseContentMode),
      m_documentFile(""),
      m_documentLoaded(false)
{
   //
}

bool DocViewerApplication::init()
{
    m_cmdLineParser = new CommandLineParser();
    bool ok = m_cmdLineParser->processArguments(arguments());
    if (!ok)
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

    if (m_cmdLineParser->pickModeEnabled())
        setDefaultUiMode(DocViewerApplication::PickContentMode);

    QObject::connect(m_contentCommunicator, SIGNAL(documentRequested()),
                     this, SLOT(switchToPickMode()));

    QObject::connect(m_contentCommunicator, SIGNAL(documentImported()),
                     this, SLOT(switchToBrowseMode()));

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

    qmlRegisterType<SortFilterDocumentModel>("DocumentViewer", 1, 0, "DocumentsModel");
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
 * \brief DocViewerApplication::createView
 * Create the master QDeclarativeView that all the pages will operate within
 */
void DocViewerApplication::createView()
{
    m_view->setTitle("Document Viewer");

    // Set ourselves up to expose functionality to run external commands from QML...
    m_view->engine()->rootContext()->setContextProperty("DOC_VIEWER", this);
    m_view->engine()->rootContext()->setContextProperty("PICKER_HUB", m_contentCommunicator);

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
        }
    }
    // sanity check
    if (qmlfile.isEmpty()) {
        qFatal("File: %s does not exist at any of the standard paths!", qPrintable(filePath));
    }

    registerHub();
    m_view->setSource(QUrl::fromLocalFile(qmlfile));
    setDocumentFile(m_cmdLineParser->documentFile());

    m_view->setResizeMode(QQuickView::SizeRootObjectToView);
    m_view->show();
}

/*!
 * \brief DocViewerApplication::setDefaultUiMode set the default UI mode. This might
 * get overridden during the lifetime
 * \param mode
 */
void DocViewerApplication::setDefaultUiMode(DocViewerApplication::UiMode mode)
{
    m_defaultUiMode = mode;
    setUiMode(mode);
}

/*!
 * \brief DocViewerApplication::setUiMode set's the current UI mode
 * \param mode
 */
void DocViewerApplication::setUiMode(DocViewerApplication::UiMode mode)
{
    bool enablePickMode = (mode == PickContentMode);

    if (enablePickMode != m_pickModeEnabled) {
        m_pickModeEnabled = enablePickMode;
        Q_EMIT pickModeEnabledChanged();
    }
}

/*!
 * \brief DocViewerApplication::pickModeEnabled returns true if the current UI
 * mode should be for picking acontent
 * \return
 */
bool DocViewerApplication::pickModeEnabled() const
{
    return m_pickModeEnabled;
}

/*!
 * \brief DocViewerApplication::switchToPickMode
 */
void DocViewerApplication::switchToPickMode()
{
    setUiMode(PickContentMode);
}

/*!
 * \brief DocViewerApplication::switchToBrowseMode
 */
void DocViewerApplication::switchToBrowseMode()
{
    Q_EMIT browseModeRequested();
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

/*!
 * \brief DocViewerApplication::returnPickedContent passes the selcted items to the
 * content manager
 * \param variant
 */
void DocViewerApplication::returnPickedContent(QList<QString> paths)
{
    QVector<QUrl> selectedMedias;
    selectedMedias.reserve(paths.size());
    foreach (const QString path, paths) {
        // We handle paths without "file://" prefix, so we need to add it when exporting to content-hub.
        selectedMedias.append(QUrl("file://" + path));
    }
    m_contentCommunicator->returnDocuments(selectedMedias);

    if (m_defaultUiMode == BrowseContentMode) {
        setUiMode(BrowseContentMode);
    } else {
        // give the app and content-hub some time to finish taks (run the event loop)
        QTimer::singleShot(10, this, SLOT(quit()));
    }
}

/*!
 * \brief DocViewerApplication::contentPickingCanceled tell the content manager, that
 * the picking was canceled
 */
void DocViewerApplication::contentPickingCanceled()
{
    m_contentCommunicator->cancelTransfer();

    if (m_defaultUiMode == BrowseContentMode) {
        setUiMode(BrowseContentMode);
    } else {
        // give the app and content-hub some time to finish taks (run the event loop)
        QTimer::singleShot(10, this, SLOT(quit()));
    }
}

void DocViewerApplication::registerHub()
{
    m_contentCommunicator->registerWithHub();
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
