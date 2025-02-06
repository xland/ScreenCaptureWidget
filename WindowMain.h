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
private:
	void printScreen();
	int x, y, w, h;
	QImage img;
};
