#include <iostream>

#include <QGuiApplication>
#include <QDir>

#include <QtQml/qqmlengine.h>
#include <QtQml/qqmlcomponent.h>
#include <QtQml/qqmlcontext.h>

#include <QtQuick/qquickitem.h>
#include <QtQuick/qquickview.h>

#include <QStringList>
#include <QProcess>
#include <qdebug.h>

#include "main.h"
#include "file.h"

int main(int argc, char *argv[])
{
    QGuiApplication launcher(argc, argv);

    QQmlEngine engine;
    QQmlComponent *component = new QQmlComponent(&engine);
    QString qmlPath;

#ifdef QT_DEBUG
    qmlPath = "ubuntu-docviewer-app.qml";
#else
    qmlPath = "/usr/share/ubuntu-docviewer-app/ubuntu-docviewer-app.qml";
#endif

    component->loadUrl(qmlPath);

    QString argument = "";
    if (launcher.arguments().size() >= 2)
	{
        int i = 0;
        for (i = 1; i < launcher.arguments().size(); i++)
		{
			qDebug() << launcher.arguments().at(i);
            if (launcher.arguments().at(i).at(0) != '-')
            {
                argument = launcher.arguments().at(i);
                break;
            }
        }

        if (i == launcher.arguments().size())
        {
            displayUsage();
            return 0;
        }
	}
	else
	{
		displayUsage();
		return 0;
	}

	QString absolutePath = QDir(argument).absolutePath();
    File *file = new File(&absolutePath);

    engine.rootContext()->setContextProperty("file", file);

    if ( !component->isReady() ) {
        qFatal("%s", qPrintable(component->errorString()));
        return -1;
    }

    QObject *topLevel = component->create();
    QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);
    QQuickView* qxView = 0;

    if (!window) {

        QQuickItem *contentItem = qobject_cast<QQuickItem *>(topLevel);

        if (contentItem) {

            qxView = new QQuickView(&engine, NULL);
            window = qxView;
            window->setFlags(Qt::Window | Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);

            qxView->setResizeMode(QQuickView::SizeRootObjectToView);
            qxView->setContent(QString("docviewer.qml"), component, contentItem);
        }
    }

    if (window)
        window->show();

    return launcher.exec();
}

void displayUsage()
{
	std::cout << "Usage :" << std::endl;
	std::cout << "ubuntu-docviewer-app <file>" << std::endl;
	return;
}
