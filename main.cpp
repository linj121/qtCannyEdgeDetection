#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "videostreamer.h"
#include <QQmlContext>
#include <paintitem.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qRegisterMetaType<cv::Mat>("cv::Mat");
    qmlRegisterType<PaintItem>("Painter", 1, 0, "PaintItem");

    VideoStreamer videoStreamer;

    engine.rootContext()->setContextProperty("VideoStreamer",&videoStreamer);

    const QUrl url(u"qrc:/OpenCV2QPainter-CMake/Main.qml"_qs);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}



