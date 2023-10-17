#ifndef WID_CHAT_INPUT_H
#define WID_CHAT_INPUT_H

#include <QDateTime>
#include <QImageReader>
#include <string>

#include "qedit_txt.h"
#include "qframe_line.h"
#include "qbutt_line.h"
#include "qmove_pos.h"
#include "wid_message.h"
#include "typedef_struct.h"

typedef long long int64;
using std::string;
using namespace typedef_struct;

//!
//! 类说明： 聊天输入框（文字输入与功能）
//!
class wid_chat_input : public qframe_line
{
    Q_OBJECT
public:
    explicit wid_chat_input(QWidget *parent = nullptr);

    int64 get_time_now();

    void init_size(QSize size);
    void set_icon(QString left,QString right);  //设置头像
    wid_message* make_msg(ct_msg_type msg);     //生成消息

signals:
    void sn_send_msg(ct_msg_type msg); //发送消息

private:
    QString _icon_left;     //对方头像
    QString _icon_right;    //我方头像

    bool is_img(const QString &file);   //判断是否为图片
    bool send_text(QString txt);        //发送信息
};

#endif // WID_CHAT_INPUT_H
