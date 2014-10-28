#include <QtQml>
#include <QtQml/QQmlContext>

#include "plugin.h"
#include "pdfModel.h"

void PopplerPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("org.ubuntu.popplerqmlplugin"));
    
    //@uri org.ubuntu.popplerqmlplugin
    qmlRegisterType<PdfModel>(uri, 1, 0, "Poppler");
}

void PopplerPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    QQmlExtensionPlugin::initializeEngine(engine, uri);
}
