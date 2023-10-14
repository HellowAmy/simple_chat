#ifndef WID_CHAT_INPUT_H
#define WID_CHAT_INPUT_H

#include <QImageReader>

#include "qedit_txt.h"
#include "qframe_line.h"
#include "qbutt_line.h"
#include "qmove_pos.h"
#include "wid_message.h"

//!
//! 类说明： 聊天输入框（文字输入与功能）
//!
class wid_chat_input : public qframe_line
{
    Q_OBJECT
public:
    explicit wid_chat_input(QWidget *parent = nullptr);

    void init_size(QSize size);
    void set_icon(QString left,QString right);  //设置头像

    wid_message* make_hint(QString txt,long long date = 0);         //提示信息
    wid_message* make_text(QString txt,Qt::AlignmentFlag flg);      //生成文字
    wid_message* make_img(QString file,Qt::AlignmentFlag flg);      //生成图片

    //生成消息
    wid_message* make_msg(wid_msg_abs *msg,Qt::AlignmentFlag flg,
                          long long date = QDateTime::currentMSecsSinceEpoch());

signals:
    void sn_send_msg(wid_message *msg); //发送消息

private:
    QString _icon_left;     //对方头像
    QString _icon_right;    //我方头像

    bool is_img(const QString &file);   //判断是否为图片
    bool send_text(QString txt);        //发送信息
};

#endif // WID_CHAT_INPUT_H
