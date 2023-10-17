#include "wid_chat.h"

#include "Tvlog.h"

wid_chat::wid_chat(QWidget *parent)
    : QWidget(parent)
{
    int space = _fsize_ioput_.space_chat;
    int max_width = _fsize_ioput_.max_width;
    int height_output = _fsize_ioput_.height_output;
    int height_input = _fsize_ioput_.height_input;

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

    connect(_input,&wid_chat_input::sn_send_msg,this,[=](ct_msg_type msg){

        qlog(msg.time);
        qlog(msg.types);
        qlog(msg.object);
        qlog(msg.content);
        qlog("\n");
        _output->add_message(_input->make_msg(msg));
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

