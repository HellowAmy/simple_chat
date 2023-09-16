#ifndef WID_FRIEND_BUTT_H
#define WID_FRIEND_BUTT_H

#include <QWidget>

#include "qframe_line.h"
#include "qbutt_line.h"
#include "qlab_img.h"
#include "qmove_pos.h"

//!
//! 类说明： 好友聊天按钮，附带图片与触发按钮
//!
class wid_friend_butt : public QWidget
{
    Q_OBJECT
public:
    explicit wid_friend_butt(QWidget *parent = nullptr);
    void set_info(QString icon,QString name);   //设置图片与名称
    void init_size(QSize size); //初始化按钮大小
    qframe_line* get_frame();   //获取边框
    qbutt_line* get_butt();     //获取按钮

signals:

private:
    int _space;             //间隔
    QString _icon;          //保存图片
    QString _name;          //保存文字
    qframe_line *_wid_icon; //图片显示
    qbutt_line *_wid_butt;  //触发按钮
};

#endif // WID_FRIEND_BUTT_H
