#ifndef WID_CHAT_H
#define WID_CHAT_H

#include <QWidget>
#include <QList>

#include "qmove_pos.h"
#include "qframe_line.h"
#include "wid_chat_input.h"
#include "wid_chat_output.h"

//!
//! 类说明： 好友聊天框，组合输入与输出，记录聊天好友信息
//!
class wid_chat : public QWidget
{
    Q_OBJECT
public:
    explicit wid_chat(QWidget *parent = nullptr);

    wid_chat_input* get_input();    //获取输入
    wid_chat_output* get_output();  //获取输出

signals:

private:
    wid_chat_input *_input;     //聊天输入
    wid_chat_output *_output;   //聊天输出
};

#endif // WID_CHAT_H
