#ifndef WID_CHAT_OUTPUT_H
#define WID_CHAT_OUTPUT_H

#include <QList>
#include <QMap>

#include "qarea_wid.h"
#include "qframe_line.h"
#include "wid_message.h"
#include "qbar_line.h"

//!
//! 类说明： 聊天输出框（显示）
//!
class wid_chat_output : public qframe_line
{
    Q_OBJECT
public:
    explicit wid_chat_output(QWidget *parent = nullptr);
    void init_size(QSize size);
    void add_message(wid_message *msg);     //加入到聊天窗口
    qarea_wid* get_area();                  //滑动窗口

signals:

private:
    qarea_wid *_area;   //滑动显示
};

#endif // WID_CHAT_OUTPUT_H
