#pragma once
#include <QPainter>
#include <QObject>
#include <QMouseEvent>
class MainWindow;
class CutMask  : public QObject
{
	Q_OBJECT

public:
	CutMask(QObject *parent=nullptr);
	~CutMask();
	void paint(QPainter* painter);
	void mouseDrag(QMouseEvent* event);
	void mousePress(QMouseEvent* event);
public:
	QRect rectMask;
private:
	MainWindow* win;
	QPoint posPress;
};
