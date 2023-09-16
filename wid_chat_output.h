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
    void add_message(wid_message *msg); //加入到聊天窗口

signals:

private:
    int _space;         //消息间隔
    int _area_width;    //显示宽度
    int _area_height;   //显示高度
    qarea_wid *_area;   //滑动显示
    QList<wid_message *> _ls_history;   //历史消息记录
//    QMap<QString,wid_message *> _map_prog;

};

#endif // WID_CHAT_OUTPUT_H
