#ifndef WID_FRIEND_LIST_H
#define WID_FRIEND_LIST_H

#include <QMap>
#include <memory>

#include "qarea_wid.h"
#include "qframe_line.h"
#include "sqlite_op.h"
#include "wid_chat.h"
#include "wid_message.h"
#include "wid_friend_butt.h"
#include "wid_friend_info.h"
#include "typedef_struct.h"
#include "files_transfer.h"
#include "qweb_files.h"

using namespace typedef_struct;
using namespace std::placeholders;

//!
//! 类说明： 聊天好友列表，显示按钮和聊天框
//!
class wid_friend_list : public qframe_line
{
    Q_OBJECT
public:
    struct ct_friend
    {
        ct_login_status status;
        wid_friend_butt *butt;
        wid_chat *chat;
    };

public:
    explicit wid_friend_list(QWidget *parent = nullptr);

    void set_login_info(int64 account,string passwd);

    void add_friend_remarks(int64 account,string remarks);
    void add_account_person(ct_ac_info ct);
    void add_friend(ct_friend ct);                      //添加好友
    void add_recv_msg(ct_swap_msg ct);                  //接收网络消息
    void update_connect(int64 account, bool online);    //更新连接状态
    void update_ac_icon(int64 account,QString path);    //重新载入账号头像
    void init_login(int64 account,QString nickname,QString icon);                   //初始化登陆
    void set_history_msg(int64 account,const std::vector<ct_msg_type> &vec_msg);    //读入聊天窗口

    wid_person_info* get_person();
    wid_friend_info* get_info();
    int64 get_account();
    string get_passwd();

signals:
    void sn_account_info(int64 friends);    //获取好友账号信息
    void sn_send_msg(ct_swap_msg ct);       //发送信息
    void sn_update_msg(int64 account);      //更新界面显示
    void sn_download_icon(int64 account);   //下载头像
    void sn_history_read_ac(int64 account,bool is_non_read = true); //读取历史记录

private:
    int _space;                     //间隔
    int64 _account;                 //账号
    string _passwd;
    QSize _size_icon;               //图片大小
    QString _icon;                  //右侧图片
    QString _path_temp;             //临时路径
    QString _path_icon;             //头像路径
    QWidget *_wid_chat;             //聊天区域
    QWidget *_wid_setting;          //设置区域
    QWidget *_wid_person;           //好友展示区域
    ct_friend *_show_friend;        //当前显示的好友窗口
    qarea_wid *_wid_area;           //好友列表滑动显示
    wid_friend_info *_wid_info;     //显示信息
    wid_person_info *_wid_fd_info;  //好友信息展示

    std::vector<QWidget*> _vec_extend;
    std::map<long long,std::shared_ptr<ct_friend>> _map_friends;    //好友列表


    //根据是否为在线状态，标记边框颜色
    void ch_connect_col(wid_friend_butt *butt,bool connect);

    //打开聊天窗口，设置按钮为选中颜色
    void ch_keep_col(wid_friend_butt *butt,bool connect);

    //更新聊天窗口句柄
    void update_show_friend(ct_friend *ct);

    //首次进入时生成聊天窗口
    void make_chat(ct_friend *ct);

    //获取好友信息窗口
    ct_friend* get_friend(int64 account);

    //显示历史信息到屏幕
    void show_msg_history(ct_friend *it,ct_msg_type ct);

    //当前窗口底部回滚
    void show_bottom_bar();

    //显示区域
    void show_wid_extend(QWidget *wid);

};

#endif // WID_FRIEND_LIST_H
