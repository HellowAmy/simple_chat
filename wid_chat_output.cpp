#include "wid_chat_output.h"

wid_chat_output::wid_chat_output(QWidget *parent)
    : qframe_line(parent)
{
    _space = 10;                //消息间隔
    _area_width = 500 + 5;      //显示宽度
    _area_height = 320;         //显示高度

    _area = new qarea_wid(this);
}

void wid_chat_output::init_size(QSize size)
{
    int padding = 5;
    this->resize_center(_area,size,padding);
    QSize size_wid = this->calc_size(size,padding);
    _area->init_size(size_wid);
}

void wid_chat_output::add_message(wid_message *msg)
{
    _area->add_wid(msg,_space);

    _ls_history.append(msg);

    if(msg->get_info().type == "Img")
    {
        qbar_line *bar = new qbar_line(msg->get_area()->get_area());
        bar->setValue(50);
//        _map_prog.insert(msg->get_info().content,msg);
    }
}
