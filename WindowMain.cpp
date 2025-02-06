#include <QTimer>
#include <QPainter>
#include <QGuiApplication>
#include <QScreen>
#include <QWindow>
#include "WindowMain.h"

WindowMain::WindowMain(QWidget* parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    x = GetSystemMetrics(SM_XVIRTUALSCREEN);
    y = GetSystemMetrics(SM_YVIRTUALSCREEN);
    w = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    h = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    printScreen();
    //ע�� ���������ô�С����ʾ���ں���ʹ��ԭ��API���ô�С
    setFixedSize(w, h);
    show();
    //ע�� ���������ô�С�����������ű���
    auto hwnd = (HWND)winId();
    SetWindowPos(hwnd, nullptr, x, y, w, h, SWP_NOZORDER | SWP_SHOWWINDOW);
    //ע�� ��������ô��ڵ�dpr������img��dpr��������������dpr���˲����������ִ��
    auto dpr = windowHandle()->devicePixelRatio();
    img.setDevicePixelRatio(dpr);
}

WindowMain::~WindowMain()
{}

void WindowMain::printScreen()
{
    //ʹ��Qt APIΪ��Ļ�������
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

void WindowMain::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.drawImage(QPoint(0, 0), img);
}
