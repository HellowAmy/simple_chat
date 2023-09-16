#include "wid_chat.h"

#include "Tvlog.h"

wid_chat::wid_chat(QWidget *parent)
    : QWidget(parent)
{
    int space = 10;
    int max_width = 500;
    int height_output = 330;
    int height_input = 160;

    _output = new wid_chat_output(this);
    _output->init_size(QSize(max_width,height_output));

    _input = new wid_chat_input(this);
    _input->init_size(QSize(max_width,height_input));

    {
        qmove_pos move;
        move.add_wid(_output);
        move.add_wid(_input);
        move.move_y(QPoint(0,0),space);
    }

    connect(_input,&wid_chat_input::sn_send_msg,this,[=](wid_message *msg){
        qlog(msg->get_info().type);
        qlog(msg->get_info().align);
        qlog(msg->get_info().time);
        qlog(msg->get_info().content);
        qlog("\n");
        _output->add_message(msg);
    });
}

wid_chat_input *wid_chat::get_input()
{
    return _input;
}

wid_chat_output *wid_chat::get_output()
{
    return _output;
}
