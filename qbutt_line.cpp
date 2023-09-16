#include "qbutt_line.h"

qbutt_line::qbutt_line(QWidget *parent)
    : QWidget(parent)
{
    this->show();
    _keep = false;
    _status = e_leave;
    _col = Qt::darkGray;
    _text = "null";
    _font = QFont("微软雅黑",10);
}

void qbutt_line::keep_status(bool keep, status st)
{
    _keep = keep;
    _kep_st = st;
    switch_status(e_leave);
}

void qbutt_line::set_col(QColor col)
{
    _col = col;
    this->update();
}

void qbutt_line::set_text(QString txt)
{
    _text = txt;
}

void qbutt_line::set_font(QFont font)
{
    _font = font;
}

QString qbutt_line::get_text()
{
    return _text;
}

QFont qbutt_line::get_font()
{
    return _font;
}

QColor qbutt_line::get_col()
{
    return _col;
}

void qbutt_line::paintEvent(QPaintEvent *)
{
    int x = 0;
    int y = 0;
    int width = this->width();
    int height = this->height();
    int space = 3;
    int radius = space *3;

    QPainter show(this);
    show.setRenderHint(QPainter::Antialiasing); // 设置抗锯齿

    //== 三状态下不同的显示 ==
    if(_status >= e_press)
    {
        QColor col(_col);
        col.setAlpha(64);
        show.setBrush(col);
    }

    if(_status >= e_enter)
    {
        show.setPen(QPen(_col, 1));
        show.drawRoundedRect(x +space, y +space, width - space*2, height - space*2, radius, radius);
    }

    if(_status >= e_leave)
    {
        show.setPen(QPen(_col, space));
        show.drawRoundedRect(x, y, width, height, radius, radius);
    }
    //== 三状态下不同的显示 ==

    show.setFont(_font);
    show.drawText(QRect(0, 0, width, height),Qt::AlignCenter, _text);
    show.end();
}

void qbutt_line::enterEvent(QEnterEvent *event)
{
    switch_status(e_enter);
    QWidget::enterEvent(event);
}

void qbutt_line::mousePressEvent(QMouseEvent *event)
{
    switch_status(e_press);
    QWidget::mousePressEvent(event);
}

void qbutt_line::leaveEvent(QEvent *event)
{
    switch_status(e_leave);
    QWidget::leaveEvent(event);
}

void qbutt_line::mouseReleaseEvent(QMouseEvent *event)
{
    if(this->rect().contains(event->pos())) { emit sn_clicked(); }

    switch_status(e_enter);
    QWidget::mouseReleaseEvent(event);
}

void qbutt_line::switch_status(status st)
{
    if(_keep)
    {
        if(st >= _kep_st) _status = st;
        else _status = _kep_st;
    }
    else _status = st;
    this->update();
}


