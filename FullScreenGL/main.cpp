#include "WindowMain.h"
#include <QtWidgets/QApplication>
#include <QOpenGLContext>
#include <QSurfaceFormat>

bool shouldUseSoftwareOpenGL() {
    QOpenGLContext context;
    context.create();

    auto moduleType = context.openGLModuleType();
    if (moduleType == QOpenGLContext::LibGL) {
        return false;  // 系统支持 OpenGL，使用默认设置
    }

    return true;  // 系统可能不支持原生 OpenGL，需要切换到 software
}


int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    //if (shouldUseSoftwareOpenGL()) {
    //    qputenv("QT_OPENGL", "software");
    //}

    //{
    //    QOpenGLContext context;
    //    if (!context.create() || !context.isValid()) {
    //        qputenv("QT_OPENGL", "software");
    //        qputenv("QT_OPENGL", "angle");
    //        qputenv("QT_ANGLE_PLATFORM", "d3d11");
    //    }
    //}


    qDebug() << "start!!!!!!!!!!!";
    WindowMain w;
    w.show();
    return a.exec();
}
