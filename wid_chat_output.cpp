#include "wid_chat_output.h"

wid_chat_output::wid_chat_output(QWidget *parent)
    : qframe_line(parent)
{
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
    int space = _fsize_ioput_.space_msg;
    _area->add_wid(msg,space);
}

qarea_wid *wid_chat_output::get_area()
{
    return _area;
}

