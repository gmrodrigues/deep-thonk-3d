#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "ui/bridge/Bridge.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(resources);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<Bridge>("com.deepthonk", 1, 0, "Bridge");

    const QUrl url(QStringLiteral("qrc:/ui/qml/Main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);

    engine.load(url);



    return app.exec();
}
