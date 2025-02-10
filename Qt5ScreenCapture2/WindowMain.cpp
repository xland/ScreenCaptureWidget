#include <QTimer>
#include <QElapsedTimer>
#include <QPainter>
#include <QGuiApplication>
#include <QScreen>
#include <QWindow>
#include <QMouseEvent>
#include <QPainterPath>
#include <qdebug.h>
#include "WindowMain.h"

WindowMain::WindowMain() : QWindow()
{
    setSurfaceType(QWindow::RasterSurface);
    setFlags(Qt::FramelessWindowHint | Qt::Window);
    x = GetSystemMetrics(SM_XVIRTUALSCREEN);
    y = GetSystemMetrics(SM_YVIRTUALSCREEN);
    w = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    h = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    printScreen();
    //注意 必须先设置大小并显示窗口后，再使用原生API重置大小
    setMaximumSize(QSize(w, h));
    setMinimumSize(QSize(w, h));
    show();
    //注意 必须先重置大小，再设置缩放比例
    auto hwnd = (HWND)winId();
    SetWindowPos(hwnd, nullptr, x, y, w, h, SWP_NOZORDER | SWP_SHOWWINDOW);
    //注意 这里必须用窗口的dpr来设置img的dpr，不能用主屏的dpr，此操作必须最后执行
    auto dpr = devicePixelRatio();
    img.setDevicePixelRatio(dpr);

    backingStore = new QBackingStore(this);
    backingStore->resize(QSize(w,h));
}

WindowMain::~WindowMain()
{
    
}

void WindowMain::renderNow()
{
    if (!isExposed()) return;
    QRect rect(0, 0, w, h);
    backingStore->beginPaint(rect);

    QPaintDevice* device = backingStore->paintDevice();
    QPainter painter(device);
    painter.drawImage(QPoint(0, 0), img);
    draw100Rect(painter);
    painter.setBrush(QColor(0, 0, 0, 120));
    QPainterPath path;
    path.addRect(-1, -1, w + 1, h + 1);
    path.addRect(rectMask);
    painter.drawPath(path);
    QColor borderColor(22, 118, 255);
    painter.setPen(QPen(QBrush(borderColor), 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(rectMask);

    painter.end();
    backingStore->endPaint();
    backingStore->flush(rect);
}

void WindowMain::printScreen()
{
    //使用Qt API为屏幕拍照亦可
    HDC hScreen = GetDC(NULL);
    HDC hDC = CreateCompatibleDC(hScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, w, h);
    DeleteObject(SelectObject(hDC, hBitmap));
    BOOL bRet = BitBlt(hDC, 0, 0, w, h, hScreen, x, y, SRCCOPY);
    img = QImage(w, h, QImage::Format_ARGB32);
    BITMAPINFO bmi = { sizeof(BITMAPINFOHEADER), (long)w, 0 - (long)h, 1, 32, BI_RGB, (DWORD)w * 4 * h, 0, 0, 0, 0 };
    GetDIBits(hDC, hBitmap, 0, h, img.bits(), &bmi, DIB_RGB_COLORS);
    DeleteDC(hDC);
    DeleteObject(hBitmap);
    ReleaseDC(NULL, hScreen);
}

void WindowMain::draw100Rect(QPainter& painter)
{
    //计算此方法的执行时间
	QElapsedTimer timer;
    timer.start();
    painter.setPen(QPen(QBrush(QColor(255, 0, 0)), 2));
    for (int i = 0; i < 1000; i++)
    {
        painter.drawRect(rand() % (int)w, rand() % (int)h, rand() % 600, rand() % 600);
    }
    qDebug() << "draw100Rect:" << timer.elapsed() << "ms";
}

//void WindowMain::paintEvent(QPaintEvent* event)
//{
//    QWidget::paintEvent(event);
//    QPainter painter(this);
//    painter.drawImage(QPoint(0, 0), img);
//
//    draw100Rect(painter);
//    painter.setBrush(QColor(0, 0, 0, 120));
//    QPainterPath path;
//    path.addRect(-1, -1, w + 1, h + 1);
//    path.addRect(rectMask);
//    painter.drawPath(path);
//    QColor borderColor(22, 118, 255);
//    painter.setPen(QPen(QBrush(borderColor), 2));
//    painter.setBrush(Qt::NoBrush);
//    painter.drawRect(rectMask);
//}

void WindowMain::exposeEvent(QExposeEvent*)
{
    if (isExposed()) {
        renderNow();
    }
}

void WindowMain::mousePressEvent(QMouseEvent* event)
{
	isPressed = true;
    posPress = event->pos();
}

void WindowMain::mouseReleaseEvent(QMouseEvent* event)
{
	isPressed = false;
}

void WindowMain::mouseMoveEvent(QMouseEvent* event)
{
    if (isPressed)
    {
        auto pos = event->pos();
        rectMask.setCoords(posPress.x(), posPress.y(), pos.x(), pos.y());
        rectMask = rectMask.normalized();
        //update();
        renderNow();
    }
}
