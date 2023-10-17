#ifndef WID_MESSAGE_H
#define WID_MESSAGE_H

#include <QWidget>

#include "qframe_line.h"
#include "qlab_img.h"
#include "qmove_pos.h"
#include "wid_msg_abs.h"
#include "typedef_struct.h"
#include "wid_msg_abs.h"

using namespace typedef_struct;

//!
//! 类说明：插入到队列的消息组合，包括在气泡中显示不同类型的消息，日期，头像
//!
typedef Qt::AlignmentFlag Qalign;
class wid_message : public QWidget
{
    Q_OBJECT
public:
    explicit wid_message(QWidget *parent = nullptr);

    //加入显示消息
    void make_msg(wid_msg_abs *msg,Qalign flg = Qt::AlignCenter,QString img = "");

    wid_msg_abs* get_area();    //获取信息区域
    qframe_line* get_tips();    //获取状态提示

private:
    int _max_width;     //最大宽度
    int _max_icon;      //头像大小
    int _space;         //两框间隔

    wid_msg_abs *_area; //信息区域
    qframe_line *_icon; //头像显示
    qframe_line *_tips; //状态提示

    qframe_line* make_icon(QString file);   //生成头像
    qframe_line* make_tips(QString file);   //生成头像
};

#endif // WID_MESSAGE_H
