#ifndef WID_FRIEND_LIST_H
#define WID_FRIEND_LIST_H

#include <QMap>
#include <memory>

#include "qarea_wid.h"
#include "qframe_line.h"
#include "wid_chat.h"
#include "wid_message.h"
#include "wid_friend_butt.h"
#include "wid_friend_info.h"

//!
//! 类说明： 聊天好友列表，显示按钮和聊天框
//!
class wid_friend_list : public qframe_line
{
    Q_OBJECT
public:
    struct ct_info
    {
        bool connect;
        long long id;
        QString name;
        QString icon;
    };
    struct ct_friend
    {
        ct_info info;
        wid_chat *chat;
        wid_friend_butt *butt;
    };
    struct ct_msg
    {
        long long id;
        long long time;
        wid_message *msg;
    };

public:
    explicit wid_friend_list(QWidget *parent = nullptr);

    void set_icon(QString icon);        //设置头像
    void add_friend(const ct_info &ct); //添加好友
    void update_connect(long long id,bool connect); //更新连接状态

signals:
    void sn_send_msg(ct_msg ct);

private:
    int _space;                     //间隔
    QSize _size_icon;               //图片大小
    QString _icon;                  //右侧图片
    QWidget *_wid_chat;             //聊天区域
    QWidget *_wid_setting;          //聊天区域
    ct_friend *_show_friend;        //当前显示的好友窗口
    qarea_wid *_wid_area;           //好友列表滑动显示
    wid_friend_info *_wid_info;     //显示信息
    QMap<long long,std::shared_ptr<ct_friend>> _map_friends;    //好友列表

    //根据是否为在线状态，标记边框颜色
    void ch_connect_col(wid_friend_butt *butt,bool connect);

    //打开聊天窗口，设置按钮为选中颜色
    void ch_keep_col(wid_friend_butt *butt,bool connect);

    //更新聊天窗口句柄
    void update_show_friend(ct_friend *ct);

    //首次进入时生成聊天窗口
    void make_chat(ct_friend *ct);

};

#endif // WID_FRIEND_LIST_H
