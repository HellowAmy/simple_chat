#ifndef WID_MESSAGE_H
#define WID_MESSAGE_H

#include <QWidget>

#include "qframe_line.h"
#include "qlab_img.h"
#include "qmove_pos.h"
#include "wid_msg_abs.h"

//!
//! 类说明：插入到队列的消息组合，包括在气泡中显示不同类型的消息，日期，头像
//!
typedef Qt::AlignmentFlag Qalign;
class wid_message : public QWidget
{
    Q_OBJECT
public:
    struct ct_info
    {
        QString type;       //类型 [文字，图片，时间]
        QString content;    //内容 [类型对应内容]
        long long time;     //时间
        Qalign align;       //对齐
    };

public:
    explicit wid_message(QWidget *parent = nullptr);

    //加入显示消息
    void make_msg(wid_msg_abs *msg,QString img,long long time,Qalign flg);
    ct_info get_info();         //获取保存信息
    wid_msg_abs* get_area();    //获取信息区域
    qframe_line* get_tips();    //获取状态提示

private:
    int _max_width;     //最大宽度
    int _max_icon;      //头像大小
    int _max_msg;       //消息宽度
    int _space;         //两框间隔
    ct_info _sava_info; //保留消息
    wid_msg_abs *_area; //信息区域
    qframe_line *_icon; //头像显示
    qframe_line *_tips; //状态提示

    qframe_line* make_icon(QString file);   //生成头像
    qframe_line* make_tips(QString file);   //生成头像
};

#endif // WID_MESSAGE_H
