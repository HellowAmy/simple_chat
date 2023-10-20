#include "wid_friend_butt.h"

wid_friend_butt::wid_friend_butt(QWidget *parent)
    : QWidget(parent)
{
    _space = 5;
}

void wid_friend_butt::set_status(ct_friends_init ct)
{
    _status = ct;
}

//void wid_friend_butt::set_icon(QString icon)
//{
//    _icon = icon;
//}

//void wid_friend_butt::set_name(QString name)
//{
//    _name = name;
//}

//void wid_friend_butt::set_remarks(QString remarks)
//{
//    _remarks = remarks;
//}

//void wid_friend_butt::set_info(QString icon, QString name)
//{
//    _icon = icon;
//    _name = name;
//}

void wid_friend_butt::init_size(QSize size)
{
    //显示头像
    QSize size_icon(size.height(),size.height());
    auto [a,b] = fn_fimg(this,stoqs(_status.icon),size_icon);
    _wid_icon = a;
    _img = b;
    _img->set_click(true);

//    _wid_icon = new qframe_line(this);
//    _img = new qlab_img(_wid_icon);
//    _img->set_img(stoqs(_status.icon) );
//    _img->set_keep(false,QSize(_wid_icon->calc_size(size_icon,5)));
//    _img->update_img();
//    _wid_icon->resize_center(_img,size_icon,5);




    QSize size_butt = size - size_icon ;
    _wid_butt = new qbutt_line(this);

    if(_status.remarks == "") _wid_butt->set_text(stoqs(_status.nickname));
    else _wid_butt->set_text(stoqs(_status.remarks));

    _wid_butt->resize(size_butt.width() - _space,size_icon.height());
    _wid_butt->show();

    this->resize(size);
    {
        qmove_pos move;
        move.add_wid(_wid_icon);
        move.add_wid(_wid_butt);
        move.move_x(QPoint(0,0),_space);
    }

    connect(_img,&qlab_img::sn_clicked,[=](){
        emit sn_account_info(_status.friends);
    });
}

void wid_friend_butt::update_butt()
{
    _img->set_img(stoqs(_status.icon));
    _img->update_img();
    _img->update();

    if(_status.remarks == "") _wid_butt->set_text(stoqs(_status.nickname));
    else _wid_butt->set_text(stoqs(_status.remarks));
    _wid_butt->update();
}

qframe_line *wid_friend_butt::get_frame()
{
    return _wid_icon;
}

qbutt_line *wid_friend_butt::get_butt()
{
    return _wid_butt;
}

ct_friends_init *wid_friend_butt::get_status()
{
    return &_status;
}
