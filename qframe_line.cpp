#include "qframe_line.h"

qframe_line::qframe_line(QWidget *parent)
    : QWidget(parent)
{
    this->show();
    _frame = true;
    _col = Qt::darkGray;
}

void qframe_line::move_center(QWidget *wid,int padding)
{
    QSize size(wid->size() + QSize(padding*2,padding*2));
    wid->move(padding,padding);
    this->resize(size);
}

void qframe_line::set_frame(bool frame)
{
    _frame = frame;
    this->update();
}

void qframe_line::set_col(QColor col)
{
    _col = col;
    this->update();
}

void qframe_line::resize_center(QWidget *wid, QSize size, int padding)
{
    QSize size_wid = calc_size(size,padding);
    this->resize(size);
    wid->resize(size_wid);
    wid->move(padding,padding);
}

QSize qframe_line::calc_size(QSize max, int padding)
{
    QSize size_wid(max - QSize(padding*2,padding*2));
    return size_wid;
}

void qframe_line::paintEvent(QPaintEvent *event)
{
    if(_frame)
    {
        int space = 3;
        int radius = space *3;

        QPainter show(this);
        show.setRenderHint(QPainter::Antialiasing);
        show.setPen(QPen(_col, space));
        show.drawRoundedRect(this->rect(), radius, radius);
        show.end();
    }
    else QWidget::paintEvent(event);
}
