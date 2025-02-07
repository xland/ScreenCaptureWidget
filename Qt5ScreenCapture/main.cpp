
#include <QtWidgets/QApplication>
#include "MainWindow.h"

void initHighDpi()
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
	QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
}
void initDefaultFont() {
	QFont font("Microsoft YaHei", 9);
	qApp->setFont(font);
}

int main(int argc, char *argv[])
{
	initHighDpi();
    QApplication a(argc, argv);
	initDefaultFont();
    MainWindow w;
    w.show();
    return a.exec();
}
