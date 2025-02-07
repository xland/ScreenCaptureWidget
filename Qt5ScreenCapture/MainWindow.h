#pragma once
#include <Windows.h>
#include <QPainterPath>
#include <QtWidgets/QMainWindow>
#include "State.h"

class PixelInfo;
class CutMask;
class MainWindow : public QObject
{
    Q_OBJECT

public:
	MainWindow(QObject* parent = nullptr);
    ~MainWindow();
	void show();
public:
    int x, y, w, h;
    QImage imgScreen;
	QImage imgBoard;
	QImage imgCanvas;
    HWND hwnd;
	State state{ State::start };
	PixelInfo* pixelInfo;
	CutMask* cutMask;
protected:
	void initWindow();
	void mousePress(QMouseEvent* event);
	void mousePressRight(QMouseEvent* event) {};
	void mouseDBClick(QMouseEvent* event) {};
	void mouseMove(QMouseEvent* event);
	void mouseDrag(QMouseEvent* event);
	void mouseRelease(QMouseEvent* event) {};
private:
	void initWinPosSize();
	void initImgs();
	void paint();
	static LRESULT CALLBACK routeWinMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK processWinMsg(UINT msg, WPARAM wParam, LPARAM lParam);
private:
};
