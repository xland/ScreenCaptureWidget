#include <QPainter>
#include <QTimer>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QGuiApplication>
#include <QScreen>
#include <QMouseEvent>
#include <QPainterPath>
#include <d2d1.h>
#include <dwmapi.h>
#include <versionhelpers.h>
#include <QWindow>
#include "WindowMain.h"


// 注意：必须使用QOpenGLWindow，不能使用QOpenGLWideget
WindowMain::WindowMain() : QOpenGLWindow()
{
    setFlags(Qt::FramelessWindowHint | Qt::Window);
	rectMask = QRect(0, 0, 0, 0);
    x = GetSystemMetrics(SM_XVIRTUALSCREEN);
    y = GetSystemMetrics(SM_YVIRTUALSCREEN);
    w = GetSystemMetrics(SM_CXVIRTUALSCREEN)+1;  //必须大一点，不然第二次update时会出现黑屏闪烁
    h = GetSystemMetrics(SM_CYVIRTUALSCREEN)+1;

    printScreen();
    //注意 必须先设置大小并显示窗口后，再使用原生API重置大小
    setMaximumSize(QSize(w, h));
    setMinimumSize(QSize(w, h));
    show();
    //注意 必须先重置大小，再设置缩放比例
    auto hwnd = (HWND)winId();
    SetWindowPos(hwnd, NULL, x, y, w, h, SWP_NOACTIVATE | SWP_SHOWWINDOW);  //HWND_TOPMOST
    //注意 这里必须用窗口的dpr来设置img的dpr，不能用主屏的dpr，此操作必须最后执行
    auto dpr = devicePixelRatio();
    img.setDevicePixelRatio(dpr);
	//enableAlphaCompositing(hwnd);
}

WindowMain::~WindowMain()
{
}

bool WindowMain::enableAlphaCompositing(HWND hWnd)
{
    if (!IsWindowsVistaOrGreater()) { return false; }
    BOOL isCompositionEnable = false;
    //检查DWM是否启用
    DwmIsCompositionEnabled(&isCompositionEnable);
    if (!isCompositionEnable) { return true; }
    DWORD currentColor = 0;
    BOOL isOpaque = false;
    //检查是否支持毛玻璃效果
    DwmGetColorizationColor(&currentColor, &isOpaque);
    if (!isOpaque || IsWindows8OrGreater())
    {
        HRGN region = CreateRectRgn(0, 0, -1, -1);
        DWM_BLURBEHIND bb = { 0 };
        bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
        bb.hRgnBlur = region;
        bb.fEnable = TRUE;
        DwmEnableBlurBehindWindow(hWnd, &bb);
        DeleteObject(region);
        return true;
    }
    else // For Window7
    {
        DWM_BLURBEHIND bb = { 0 };
        bb.dwFlags = DWM_BB_ENABLE;
        DwmEnableBlurBehindWindow(hWnd, &bb);
        return false;
    }
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

void WindowMain::paintGL()
{
 //   QOpenGLPaintDevice device(QSize(w,h));
	//device.setDevicePixelRatio(devicePixelRatio());
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawImage(QPoint(0, 0), img);
    painter.setBrush(QColor(0, 0, 0, 120));
    QPainterPath path;
    path.addRect(-1, -1, w + 1, h + 1);
    path.addRect(rectMask);
    painter.drawPath(path);
    QColor borderColor(22, 118, 255);
    painter.setPen(QPen(QBrush(borderColor), 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(rectMask);
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
		update(rectMask);
    }
}
