#include "wid_chat_input.h"

wid_chat_input::wid_chat_input(QWidget *parent)
    : qframe_line(parent)
{

}

int64 wid_chat_input::get_time_now()
{
    return QDateTime::currentMSecsSinceEpoch();
}

void wid_chat_input::init_size(QSize size)
{
    int padding = _fsize_ioput_.input_padding;
    int size_extend = _fsize_ioput_.input_extend_butt;

    //显示区
    QWidget *wid = new QWidget(this);
    this->resize_center(wid,size,padding);

    //扩展区
    QWidget *wid_extend = new QWidget(wid);
    wid_extend->resize(wid->width(),size_extend);

    //扩展区--发送按钮
    qbutt_line *butt_send = new qbutt_line(wid_extend);
    butt_send->resize(size_extend,size_extend);
    butt_send->set_text("发送");
    butt_send->move(wid_extend->width() - butt_send->width(),0);

    //输入框
    qframe_line *wid_frame = new qframe_line(wid);
    qedit_txt *edit_text = new qedit_txt(wid_frame);
    wid_frame->resize_center(edit_text,QSize(wid->width(),wid->height() - size_extend - padding),padding);

    //显示区布局
    {
        qmove_pos move;
        move.add_wid(wid_extend);
        move.add_wid(wid_frame);
        move.move_y(QPoint(0,0),padding);
    }

    //消息转发--发送按钮
    connect(butt_send,&qbutt_line::sn_clicked,this,[=](){

        QString txt = edit_text->get_send_text();
        if(txt != "")
        {
            ct_msg_type msg {get_time_now(),_types_Text_,_object_AL_,txt.toStdString()};
            emit sn_send_msg(msg);
        }


//        send_text(edit_text->get_send_text());
    });

    //消息转发--回车
    connect(edit_text,&qedit_txt::sn_send_enter,this,[=](){
        send_text(edit_text->get_send_text());
    });

    //消息转发--拖入图片
    connect(edit_text,&qedit_txt::sn_send_drop,this,[=](QList<QUrl> ls){
        for(const auto &a:ls)
        {
            QString file = a.toLocalFile();
            ct_msg_type msg {get_time_now(),_types_File_,_object_AR_,file.toStdString()};
            if(is_img(file)) emit sn_send_msg(msg);
        }
    });
}

void wid_chat_input::set_icon(QString left, QString right)
{
    _icon_left = left;
    _icon_right = right;
}

wid_message *wid_chat_input::make_msg(ct_msg_type ct)
{
    wid_message *wid = new wid_message;
    wid_msg_abs *msg = nullptr;
    QString icon = "";
    Qalign flg = Qt::AlignCenter;

    if(ct.types == _types_Text_)       msg = new wid_msg_text;
    else if(ct.types == _types_Hint_)  msg = new wid_msg_hint;
    else if(ct.types == _types_Img_)   msg = new wid_msg_img;

    if(ct.object == _object_AL_)
    {
        flg = Qt::AlignLeft;
        icon = _icon_left;
    }
    else if(ct.object == _object_AR_)
    {
        flg = Qt::AlignRight;
        icon = _icon_right;
    }

    int max_width = _fsize_message_.msg_width;
    int padding = _fsize_message_.msg_padding;
    msg->update_msg(stoqs(ct.content),max_width,padding);
    wid->make_msg(msg,flg,icon);
    return wid;
}

bool wid_chat_input::is_img(const QString &file)
{
    QImageReader img(file);
    return img.canRead();
}

bool wid_chat_input::send_text(QString txt)
{
    if(txt != "")
    {
        ct_msg_type msg {get_time_now(),_types_Text_,_object_AR_,txt.toStdString()};
        emit sn_send_msg(msg);
        return true;
    }
    else return false;
}
