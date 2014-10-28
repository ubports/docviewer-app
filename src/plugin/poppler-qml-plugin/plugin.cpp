#include <QtQml>
#include <QtQml/QQmlContext>

#include "plugin.h"
#include "pdfModel.h"

void PopplerPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("com.ubuntu.popplerqmlplugin"));
    
    //@uri com.ubuntu.popplerqmlplugin
    qmlRegisterType<PdfModel>(uri, 1, 0, "Poppler");
}

void PopplerPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    QQmlExtensionPlugin::initializeEngine(engine, uri);
}
