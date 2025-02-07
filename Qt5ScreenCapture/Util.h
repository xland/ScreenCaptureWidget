#pragma once
#include <Windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <QMouseEvent>
#include <QImage>

class Util
{
public:
	static QMouseEvent createMouseEvent(const QEvent::Type& type = QEvent::MouseMove, const Qt::MouseButton& btn = Qt::MouseButton::LeftButton);
	static QMouseEvent createMouseEvent(const LPARAM& lParam, const QEvent::Type& type, const Qt::MouseButton& btn = Qt::MouseButton::LeftButton);
	static QImage printScreen(const int& x, const int& y, const int& w, const int& h);
	static void imgToClipboard(const QImage& img);
	static void setClipboardText(const std::wstring& text);
	static void copyColor(const int& key);
	static bool posInScreen(const int& x, const int& y);
};

