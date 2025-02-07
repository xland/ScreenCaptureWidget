#pragma once

#include <Windows.h>
#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QImage>
#include <QOpenGLPaintDevice>
#include <QOpenGLTexture>

class WindowMain : public QOpenGLWindow, public QOpenGLFunctions
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
	void showEvent(QShowEvent* event) override;
	void initializeGL() override;
private:
	void printScreen();
	int x, y, w, h;
	QImage img;
	bool isPressed = false;
	QPoint posPress;
	QRect rectMask;
	QOpenGLTexture* texture;
};
