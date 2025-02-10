#pragma once
#include <Windows.h>
#include <QWindow>
#include <QImage>
#include <QPainter>
#include <QBackingStore>

class WindowMain  : public QWindow
{
	Q_OBJECT

public:
	WindowMain();
	~WindowMain();
protected:
	//void paintEvent(QPaintEvent* event) override;
	void exposeEvent(QExposeEvent*) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
private:
	void renderNow();
	void printScreen();
	void draw100Rect(QPainter& painter);
	int x, y, w, h;
	QImage img;
	bool isPressed = false;
	QPoint posPress;
	QRect rectMask;
	QBackingStore* backingStore;
};
