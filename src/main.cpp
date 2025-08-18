#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "ui/bridge/Bridge.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    Bridge bridge;
    engine.rootContext()->setContextProperty("bridge", &bridge);

    const QUrl url(QStringLiteral("qrc:/ui/qml/Main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);

    engine.load(url);

    // Connect the C++ signal to a QML slot/function
    QObject* rootObject = engine.rootObjects().first();
    QObject::connect(&bridge, &Bridge::rogerianReply,
                     rootObject, [rootObject](const QString &reply) {
        QMetaObject::invokeMethod(rootObject->findChild<QObject*>("log"), "append",
                                  Q_ARG(QString, "Therapist: " + reply + "\n"));
    });

    return app.exec();
}
