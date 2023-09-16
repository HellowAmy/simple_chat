#include "wid_msg_abs.h"

wid_msg_abs::wid_msg_abs(QWidget *parent)
    : qframe_line(parent)
{
    _padding = 5;
}

QString wid_msg_abs::get_type()
{
    return _type;
}

QString wid_msg_abs::get_content()
{
    return _content;
}

QWidget *wid_msg_abs::get_area()
{
    return _wid_area;
}

wid_msg_text::wid_msg_text(QString txt,QWidget *parent)
    : wid_msg_abs(parent)
{
    _type = "Text";
    _content = txt;

    _lab = new qlab_wrap(this);
    _lab->set_text(txt);
    _wid_area = _lab;
}

void wid_msg_text::update_msg(int max_width)
{
    int size_wid = max_width - _padding*2;
    _lab->set_width_max(size_wid);
    _lab->update_text();
    this->move_center(_lab,_padding);
}

wid_msg_img::wid_msg_img(QString file, QWidget *parent)
    : wid_msg_abs(parent)
{
    _type = "Img";
    _content = file;

    _lab = new qlab_img(this);
    _lab->set_img(file);
    _wid_area = _lab;
}

void wid_msg_img::update_msg(int max_width)
{
    QPixmap pix(_lab->get_file());
    int width = pix.width() > max_width ? max_width : pix.width();
    width -= _padding*2;

    _lab->set_keep(true,QSize(width,width));
    _lab->update_img();
    this->move_center(_lab,_padding);
}

wid_msg_hint::wid_msg_hint(QString hint, QWidget *parent)
    : wid_msg_abs(parent)
{
    this->set_frame(false);
    _type = "Hint";
    _content = hint;

    _lab = new qlab_wrap(this);
    _lab->set_text(hint);
    _lab->set_font(QFont("微软雅黑",10));
    _wid_area = _lab;
}

void wid_msg_hint::update_msg(int max_width)
{
    int size_wid = max_width - _padding*2;
    _lab->set_width_max(size_wid);
    _lab->update_text();
    this->move_center(_lab,0);
}
