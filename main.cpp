#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "lobby.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QCoreApplication::setOrganizationName("Clayground");
    QCoreApplication::setOrganizationDomain("clayground.com");
    QCoreApplication::setApplicationName("Test");

    QGuiApplication app(argc, argv);

    qmlRegisterType<Lobby>("Lobby", 1, 0, "Lobby");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    Lobby *lobby = new Lobby();
    lobby->start();

    return app.exec();
}
