#ifndef WID_FRIEND_BUTT_H
#define WID_FRIEND_BUTT_H

#include <QWidget>

#include "qframe_line.h"
#include "qbutt_line.h"
#include "qlab_img.h"
#include "qmove_pos.h"
#include "typedef_struct.h"

using namespace typedef_struct;

//!
//! 类说明： 好友聊天按钮，附带图片与触发按钮
//!
class wid_friend_butt : public QWidget
{
    Q_OBJECT
public:
    explicit wid_friend_butt(QWidget *parent = nullptr);
    void set_status(ct_friends_init ct);
//    void set_icon(QString icon);
//    void set_name(QString name);
//    void set_remarks(QString remarks);
//    void set_info(QString icon,QString name);   //设置图片与名称
    void init_size(QSize size);     //初始化按钮大小
    void update_butt();             //更新头像
    qframe_line* get_frame();       //获取边框
    qbutt_line* get_butt();         //获取按钮
    ct_friends_init* get_status();  //获取状态信息

signals:
    void sn_account_info(int64 account);

private:
    int _space;                 //间隔
    ct_friends_init _status;    //账号状态
    qframe_line *_wid_icon;     //图片显示
    qbutt_line *_wid_butt;      //触发按钮
    qlab_img *_img;             //头像显示
};

#endif // WID_FRIEND_BUTT_H
