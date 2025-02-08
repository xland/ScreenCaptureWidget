#include "WindowMain.h"
#include <QtWidgets/QApplication>
#include <QOpenGLContext>


int main(int argc, char *argv[])
{
    
    QApplication a(argc, argv);
    //{
    //    QOpenGLContext context;
    //    if (!context.create() || !context.isValid()) {
    //        qputenv("QT_OPENGL", "software");
    //    }
    //}
    WindowMain w;
    w.show();
    return a.exec();
}
