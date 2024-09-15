#include "widget.h"

#include <QApplication>
#include <QFontDatabase>
#include <QFile>

int main(int argc, char *argv[])
{
    if (qEnvironmentVariableIsEmpty("QTGLESSTREAM_DISPLAY"))
    {
        qputenv("QT_QPA_EGLFS_PHYSICAL_WIDTH", QByteArray("213"));
        qputenv("QT_QPA_EGLFS_PHYSICAL_HEIGHT", QByteArray("120"));

        #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
        #endif
    }

    QApplication app(argc, argv);

    QFile resFile(":/style/Aqua.qss");
    if(resFile.open(QIODevice::ReadOnly))
        app.setStyleSheet(resFile.readAll());
    resFile.close();

    // 设置全局字体，用于解决Qt for WebAssembly中文显示异常
    int fontId = QFontDatabase::addApplicationFont(":/font/Alibaba-PuHuiTi-Light.otf");
    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    qDebug() << "fontfamilies:" << fontFamilies;
    if (fontFamilies.size() > 0)
    {
        QFont font;
        font.setFamily(fontFamilies[0]);//设置全局字体
        app.setFont(font);
    }

    widget w;
    w.show();
    return app.exec();
}