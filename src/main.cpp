#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "ui/bridge/Bridge.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(resources);

    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    Bridge *bridge = new Bridge(&engine);
    engine.rootContext()->setContextProperty("bridge", bridge);

    const QUrl url(QStringLiteral("qrc:/ui/qml/Main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);

    engine.load(url);



    return app.exec();
}
