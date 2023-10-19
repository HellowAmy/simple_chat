#include "qlab_img.h"

qlab_img::qlab_img(QWidget *parent)
    : QWidget(parent)
{
    this->show();
    _frame = false;
    _click = false;
}

void qlab_img::set_frame(bool frame)
{
    _frame = frame;
}

void qlab_img::set_img(QString file)
{
    _file = file;
}

void qlab_img::set_keep(bool keep, QSize size)
{
    _keep = keep;
    _size = size;
}

void qlab_img::set_click(bool click)
{
    _click = click;
}

void qlab_img::update_img()
{
    if(_pix.load(_file))
    {
        QSize size;
        if(_size.width() != 0) size = _size;
        else size = _pix.size();

        if(_keep) _pix = _pix.scaled(size,Qt::KeepAspectRatio,Qt::SmoothTransformation);
        else _pix = _pix.scaled(size,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        this->resize(_pix.size());
    }
    this->update();
}

QString qlab_img::get_file()
{
    return _file;
}

void qlab_img::paintEvent(QPaintEvent *event)
{
    QPainter show(this);

    if(_pix.isNull() == false)
    { show.drawPixmap(this->rect(),_pix); }

    if(_frame || _pix.isNull())
    {
        show.setPen(QPen(Qt::darkGray,2));
        show.setRenderHint(QPainter::Antialiasing);
        show.drawRect(this->rect());
        show.drawLine(QPoint(0,0),QPoint(this->width(),this->height()));
        show.drawLine(QPoint(this->width(),0),QPoint(0,this->height()));
    }
    show.end();
}

void qlab_img::enterEvent(QEnterEvent *event)
{
    if(_click) this->setCursor(Qt::PointingHandCursor);
    QWidget::enterEvent(event);
}

void qlab_img::leaveEvent(QEvent *event)
{
    if(_click) this->setCursor(Qt::ArrowCursor);
    QWidget::leaveEvent(event);
}

void qlab_img::mouseReleaseEvent(QMouseEvent *event)
{
    if(_click && this->rect().contains(event->pos())) { emit sn_clicked(); }
    QWidget::mouseReleaseEvent(event);
}
