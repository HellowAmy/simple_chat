#include "wid_friend_butt.h"

wid_friend_butt::wid_friend_butt(QWidget *parent)
    : QWidget(parent)
{
    _space = 5;
}

void wid_friend_butt::set_info(QString icon, QString name)
{
    _icon = icon;
    _name = name;
}

void wid_friend_butt::init_size(QSize size)
{
    QSize size_icon(size.height(),size.height());
    _wid_icon = new qframe_line(this);
    qlab_img *img = new qlab_img(_wid_icon);
    img->set_img(_icon);
    img->set_keep(false,QSize(_wid_icon->calc_size(size_icon,5)));
    img->update_img();
    _wid_icon->resize_center(img,size_icon,5);

    QSize size_butt = size - size_icon ;
    _wid_butt = new qbutt_line(this);
    _wid_butt->set_text(_name);
    _wid_butt->resize(size_butt.width() - _space,size_icon.height());
    _wid_butt->show();

    this->resize(size);
    {
        qmove_pos move;
        move.add_wid(_wid_icon);
        move.add_wid(_wid_butt);
        move.move_x(QPoint(0,0),_space);
    }
}

qframe_line *wid_friend_butt::get_frame()
{
    return _wid_icon;
}

qbutt_line *wid_friend_butt::get_butt()
{
    return _wid_butt;
}
