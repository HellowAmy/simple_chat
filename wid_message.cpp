#include "wid_message.h"

wid_message::wid_message(QWidget *parent)
    : QWidget(parent)
{
    _space      = _fsize_message_.msg_space;
    _max_width  = _fsize_message_.msg_max_width;
    _max_icon   = _fsize_message_.msg_icon;
}

void wid_message::make_msg(wid_msg_abs *msg, Qalign flg, QString img)
{
    //设置父类,加入到消息框,必须手动显示
    msg->setParent(this);
    msg->show();

    if(flg == Qt::AlignCenter && img == "")
    {
        msg->move(_max_width/2 - msg->width()/2,0);
        this->resize(_max_width,msg->height());
    }
    else
    {
        int width_msg = msg->width() + _space*2 + _max_icon*2;
        _icon = make_icon(img);
        _tips = make_tips("");

        qmove_pos move;
        if(flg == Qt::AlignLeft)
        {
            move.add_wid(_icon);
            move.add_wid(msg);
            move.add_wid(_tips);
            move.move_x(QPoint(0,0),_space);
        }
        else if(flg == Qt::AlignRight)
        {
            move.add_wid(_icon);
            move.add_wid(msg);
            move.add_wid(_tips);
            move.move_xr(QPoint(_max_width - width_msg,0),_space);
        }
        this->resize(_max_width,msg->height() > _max_icon ? msg->height() : _max_icon);
    }
}


wid_msg_abs *wid_message::get_area()
{
    return _area;
}

qframe_line *wid_message::get_tips()
{
    return _tips;
}

qframe_line *wid_message::make_icon(QString file)
{
    qframe_line *icon = new qframe_line(this);
    QSize size_max(_max_icon,_max_icon);
    QSize size_icon = icon->calc_size(size_max,_space);
    qlab_img *lab = new qlab_img(icon);
    lab->set_img(file);
    lab->set_keep(false,size_icon);
    lab->update_img();
    icon->resize_center(lab,size_max,_space);
    return icon;
}

qframe_line *wid_message::make_tips(QString file)
{
    qframe_line *tips = new qframe_line(this);
    tips->resize(QSize(_max_icon,_max_icon));
    tips->set_frame(false);
    return tips;
}
