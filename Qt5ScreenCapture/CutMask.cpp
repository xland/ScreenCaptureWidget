#include <QPainterPath>
#include "CutMask.h"
#include "MainWindow.h"

CutMask::CutMask(QObject* parent) : QObject(parent), win{ (MainWindow*)parent }
{

}

CutMask::~CutMask()
{}

void CutMask::paint(QPainter * painter)
{
    painter->setBrush(QColor(0, 0, 0, 120));
    QPainterPath path;
    path.addRect(-1, -1, win->w + 1, win->h + 1);
    path.addRect(rectMask);
    painter->drawPath(path);
    QColor borderColor(22, 118, 255);
    painter->setPen(QPen(QBrush(borderColor), 2));
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(rectMask);
}

void CutMask::mouseDrag(QMouseEvent* event)
{
    auto pos = event->pos();
    rectMask.setCoords(posPress.x(),posPress.y(), pos.x(), pos.y());
    rectMask = rectMask.normalized();
    InvalidateRect(win->hwnd, nullptr, false);
}

void CutMask::mousePress(QMouseEvent* event)
{
    posPress = event->pos();
    win->state = State::mask;
}
