#include "qlab_bubble.h"

qlab_bubble::qlab_bubble(QWidget *parent)
    : QWidget(parent)
{
    _radius = 5;
    _col_backdrop = QColor(0xADD8E6);
}

void qlab_bubble::set_col_backdrop(QColor col)
{
    _col_backdrop = col;
}

void qlab_bubble::paintEvent(QPaintEvent *)
{
    QPainter show(this);
    show.setRenderHint(QPainter::Antialiasing);
    show.setPen(Qt::NoPen);
    show.setBrush(_col_backdrop);
    show.drawRoundedRect(this->rect(),_radius,_radius);
    show.end();
}
