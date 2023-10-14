#include "wid_chat_input.h"

wid_chat_input::wid_chat_input(QWidget *parent)
    : qframe_line(parent)
{

}

void wid_chat_input::init_size(QSize size)
{
    int padding = 5;
    int size_extend = 50;

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
        send_text(edit_text->get_send_text());
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
            if(is_img(file)) emit sn_send_msg(make_img(file,Qt::AlignRight));
        }
    });
}

void wid_chat_input::set_icon(QString left, QString right)
{
    _icon_left = left;
    _icon_right = right;
}

wid_message *wid_chat_input::make_hint(QString txt,long long date)
{
    if(date == 0) return make_msg(new wid_msg_hint(txt),Qt::AlignCenter);
    return make_msg(new wid_msg_hint(txt),Qt::AlignCenter,date);
}

wid_message *wid_chat_input::make_text(QString txt, Qt::AlignmentFlag flg)
{
    return make_msg(new wid_msg_text(txt),flg);
}

wid_message *wid_chat_input::make_img(QString file, Qt::AlignmentFlag flg)
{
    return make_msg(new wid_msg_img(file),flg);
}

wid_message *wid_chat_input::make_msg(wid_msg_abs *msg, Qt::AlignmentFlag flg,long long date)
{
//    QString date = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
//    long long date = QDateTime::currentDateTime().toMSecsSinceEpoch();
    wid_message *wid = new wid_message;
    if(flg == Qt::AlignRight) wid->make_msg(msg,_icon_right,date,flg);
    else if(flg == Qt::AlignLeft) wid->make_msg(msg,_icon_left,date,flg);
    else if(flg == Qt::AlignCenter) wid->make_msg(msg,"",date,flg);
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
        emit sn_send_msg(make_text(txt,Qt::AlignRight));
        return true;
    }
    else return false;
}
