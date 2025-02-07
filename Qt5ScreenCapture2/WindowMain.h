#pragma once
#include <Windows.h>
#include <QWidget>
#include <QImage>

class WindowMain  : public QWidget
{
	Q_OBJECT

public:
	WindowMain(QWidget* parent = nullptr);
	~WindowMain();
protected:
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
private:
	void printScreen();
	int x, y, w, h;
	QImage img;
	bool isPressed = false;
	QPoint posPress;
	QRect rectMask;
};
