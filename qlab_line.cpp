#include "qlab_line.h"

qlab_line::qlab_line(QWidget *parent)
    : QWidget(parent)
{
    _col = Qt::darkGray;
}

void qlab_line::paintEvent(QPaintEvent *event)
{
    int space = 3;
    int radius = space *3;

    QPainter show(this);
    show.setRenderHint(QPainter::Antialiasing);
    show.setPen(QPen(_col, space));
    show.drawRoundedRect(this->rect(), radius, radius);
    show.end();
}
