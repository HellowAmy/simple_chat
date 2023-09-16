#include "wid_message.h"

wid_message::wid_message(QWidget *parent)
    : QWidget(parent)
{
    _space = 5;
    _max_width = 490 - _space;
    _max_icon = 50;
    _max_msg = _max_width - _max_icon*2 - _space*2;
}

void wid_message::make_msg(wid_msg_abs *msg, QString img, long long time, Qalign flg)
{
    //设置父类,加入到消息框,必须手动显示
    msg->setParent(this);
    msg->update_msg(_max_msg);
    msg->show();
    _area = msg;

    //消息框布局
    if(img != "") //带头像（发送消息，设置对齐方式）
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
            move.add_wid(_tips);
            move.add_wid(msg);
            move.add_wid(_icon);
            move.move_x(QPoint(_max_width - width_msg,0),_space);
        }
        this->resize(_max_width,msg->height() > _max_icon ? msg->height() : _max_icon);
    }
    else //无头像（提示信息，居中）
    {
        msg->move(_max_width/2 - msg->width()/2,0);
        this->resize(_max_width,msg->height());
    }

    _sava_info.align = flg;
    _sava_info.type = msg->get_type();
    _sava_info.content = msg->get_content();
    _sava_info.time = time;
}

wid_message::ct_info wid_message::get_info()
{
    return _sava_info;
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
    auto fn_make_img = [=](QWidget *parent, QString file,int width,int padding){
        qframe_line *icon = new qframe_line(parent);
        QSize size_max(width,width);
        QSize size_icon = icon->calc_size(size_max,padding);
        qlab_img *lab = new qlab_img(icon);
        lab->set_img(file);
        lab->set_keep(false,size_icon);
        lab->update_img();
        icon->resize_center(lab,size_max,_space);
        return icon;
    };

    return fn_make_img(this,file,_max_icon,5);
}

qframe_line *wid_message::make_tips(QString file)
{
    qframe_line *tips = new qframe_line(this);
    tips->resize(QSize(_max_icon,_max_icon));
    tips->set_frame(false);
    return tips;
}
