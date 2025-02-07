#pragma once

#include <Windows.h>
#include <QOpenGLWindow>
#include <QImage>
#include <QOpenGLPaintDevice>
#include <QOpenGLTexture>

class WindowMain : public QOpenGLWindow
{
	Q_OBJECT

public:
	WindowMain();
	~WindowMain();
protected:
	void paintGL() override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
private:
	void printScreen();
	float x, y, w, h;
	QImage img;
	bool isPressed = false;
	QPoint posPress;
	QRect rectMask;
};
