
#include <QtWidgets/QApplication>
#include "WindowMain.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WindowMain w;
    w.show();
    return a.exec();
}
