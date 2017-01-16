#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "wsclient.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    QObject *item = engine.rootObjects().at(0);

    WSClient client(QUrl(QStringLiteral("ws://192.168.4.1:81")));
    QObject::connect(&client, &WSClient::closed, &app, &QCoreApplication::quit);

    QObject::connect(item, SIGNAL(changeColor(QString)), &client, SLOT(onColorChanged(QString)));

    return app.exec();
}
