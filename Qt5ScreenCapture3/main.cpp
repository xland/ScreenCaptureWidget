#include <QtWidgets/QApplication>
#include "WindowMain.h"

void initHighDpi()
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
}

int main(int argc, char *argv[])
{
    initHighDpi();
    QApplication a(argc, argv);
    WindowMain w;
    w.show();
    return a.exec();
}
