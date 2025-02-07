﻿#include <QMessageBox>
#include <QtWidgets/QApplication>
#include <QScreen>
#include "Util.h"

QMouseEvent Util::createMouseEvent(const QEvent::Type& type, const Qt::MouseButton& btn)
{
    POINT pos; 
    GetCursorPos(&pos);
    auto isCtrl = GetKeyState(VK_CONTROL) & 0x8000;
    auto isShift = GetKeyState(VK_SHIFT) & 0x8000;
    Qt::KeyboardModifiers mf = Qt::NoModifier;
    if (isCtrl) mf = Qt::ControlModifier;
    if (isShift) mf = mf | Qt::ShiftModifier;
    QPointF p(pos.x, pos.y);
    QPointF g(pos.x, pos.y);
    return QMouseEvent(type, p, g, btn, btn, mf);
}

QMouseEvent Util::createMouseEvent(const LPARAM& lParam, const QEvent::Type& type, const Qt::MouseButton& btn)
{
    auto x = GET_X_LPARAM(lParam);
    auto y = GET_Y_LPARAM(lParam); 
    auto isCtrl = GetKeyState(VK_CONTROL) & 0x8000;
    auto isShift = GetKeyState(VK_SHIFT) & 0x8000;
    Qt::KeyboardModifiers mf = Qt::NoModifier;
    if (isCtrl) mf = Qt::ControlModifier;
    if (isShift) mf = mf | Qt::ShiftModifier;
    QPointF p(x, y);
    POINT pos;
    GetCursorPos(&pos);  //比ClientToScreen 效率要高
    QPointF g(pos.x, pos.y);
    return QMouseEvent(type, p, g, btn, btn, mf);
}
void Util::imgToClipboard(const QImage& img)
{
    auto width = img.width();
    auto height = img.height();
    HDC screenDC = GetDC(nullptr);
    HDC memoryDC = CreateCompatibleDC(screenDC);
    HBITMAP hBitmap = CreateCompatibleBitmap(screenDC, width, height);
    DeleteObject(SelectObject(memoryDC, hBitmap));
    BITMAPINFO bmi = { sizeof(BITMAPINFOHEADER), width, 0 - height, 1, 32, BI_RGB, width * 4 * height, 0, 0, 0, 0 };
    SetDIBitsToDevice(memoryDC, 0, 0, width, height, 0, 0, 0, height, img.bits(), &bmi, DIB_RGB_COLORS);
    if (!OpenClipboard(nullptr)) {
        QMessageBox::warning(NULL, "Error", "Failed to open clipboard when save to clipboard.", QMessageBox::StandardButton::Ok);
        return;
    }
    EmptyClipboard();
    SetClipboardData(CF_BITMAP, hBitmap);
    CloseClipboard();
    ReleaseDC(nullptr, screenDC);
    DeleteDC(memoryDC);
    DeleteObject(hBitmap);
}

void Util::setClipboardText(const std::wstring& text) {
    OpenClipboard(NULL);
    EmptyClipboard();
    size_t len = (text.size() + 1) * sizeof(wchar_t);
    HANDLE copyHandle = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, len);
    if (copyHandle == NULL)
    {
        MessageBox(NULL, L"Failed to alloc clipboard memory.", L"Error", MB_OK | MB_ICONERROR);
        CloseClipboard();
        return; // 处理错误
    }
    byte* copyData = reinterpret_cast<byte*>(GlobalLock(copyHandle));
    if (copyData) {
        memcpy(copyData, text.data(), len);
    }
    GlobalUnlock(copyHandle);
    SetClipboardData(CF_UNICODETEXT, copyHandle);
    CloseClipboard();
}

void Util::copyColor(const int& key)
{
    std::wstring tarStr;
    HDC hdcScreen = GetDC(NULL);
    POINT pos;
    GetCursorPos(&pos);
    COLORREF colorNative = GetPixel(hdcScreen, pos.x, pos.y);
    ReleaseDC(NULL, hdcScreen);
    if (key == 0) {
        QColor color(GetRValue(colorNative), GetGValue(colorNative), GetBValue(colorNative));
        tarStr = color.name().toUpper().toStdWString();
    }
    else if (key == 1) {
        tarStr = QString("%1,%2,%3").arg(GetRValue(colorNative)).arg(GetGValue(colorNative)).arg(GetBValue(colorNative)).toStdWString();
    }
    else if (key == 2) {
        QColor cmyk(GetRValue(colorNative), GetGValue(colorNative), GetBValue(colorNative));
        tarStr = QString("%1,%2,%3,%4").arg(cmyk.cyan()).arg(cmyk.magenta()).arg(cmyk.yellow()).arg(cmyk.black()).toStdWString();
    }
    Util::setClipboardText(tarStr);
}

QImage Util::printScreen(const int& x, const int& y, const int& w, const int& h)
{
    auto screens = QGuiApplication::screens();
    for (int i = 0; i < screens.count(); i++) {
        auto screen = screens[i];
        QRect _screenRect = screen->geometry();
        if (_screenRect.x() == 0 && _screenRect.y() == 0) {
            auto dpr = screen->devicePixelRatio();
            auto img = screen->grabWindow(0, x / dpr, y / dpr, w / dpr, h / dpr).toImage();
            img.convertToFormat(QImage::Format_ARGB32_Premultiplied);
            return img;
        }
    }
}
bool Util::posInScreen(const int& x, const int& y)
{
    QList<QScreen*> screens = QGuiApplication::screens();
    for (QScreen* screen : screens) {
        if (screen->geometry().contains(x, y)) {
            return true;
        }
    }
    return false;
}
