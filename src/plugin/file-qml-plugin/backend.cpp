#include <QtQml>
#include <QtQml/QQmlContext>

#include "backend.h"
#include "docviewerFile.h"
#include "fileReader.h"

void BackendPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("org.ubuntu.fileqmlplugin"));
    
    //@uri org.ubuntu.fileqmlplugin

    qmlRegisterType<DocviewerFile>(uri, 1, 0, "File");
    qmlRegisterType<FileReader>(uri, 1, 0, "FileReader");    

}

void BackendPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    QQmlExtensionPlugin::initializeEngine(engine, uri);
}
