#ifndef QWEB_CLIENT_H
#define QWEB_CLIENT_H

#include <QObject>
#include <QQueue>
#include <QMap>
#include <QIterator>
#include <QDateTime>
#include <functional>
#include <thread>
#include <condition_variable>

#include "include/hv/WebSocketClient.h"
#include "web_protocol.h"
#include "inter_client.h"
#include "qweb_files.h"
#include "sqlite_op.h"
#include "typedef_struct.h"

using namespace typedef_struct;
using namespace protocol;
using std::function;
using std::string;
using hv::WebSocketClient;


//!
//! 类说明： 网络连接类，提供登陆注册等与服务器对接功能
//!
class qweb_client : public QObject
{
    Q_OBJECT
public:
    explicit qweb_client(QObject *parent = nullptr);
    ~qweb_client();

    int open(string ip = CS_SERVER_ADDRESS,int port = CS_PORT_TASKS,string txt = protocol::_head_);
    void close();

    //!
    //! 登陆逻辑：
    //!     1.发送登陆请求,获取账号信息与好友初始化信息
    //!     2.打开历史记录数据库，创建好友数据表
    //!     3.拉取交换暂存信息，获取暂存信息并加入到历史记录
    //!
    bool ask_login(int64 account,string passwd);

    //!
    //! 注册逻辑：
    //!     1.提交信息
    //!     2.反馈账号密码信息
    //!
    bool ask_register(int64 phone,int64 age,int64 sex,string nickname,string location,string passwd);

    //!
    //! 交换逻辑：
    //!     1.发送信息，加入发送方历史记录
    //!     2.服务器转发信息，不在线则加入暂存                  [服务器处理]
    //!     3.对方账号接收信息，发送确认反馈，加入接收方历史记录   [对方处理]
    bool ask_swap_msg(ct_swap_msg msg);
    bool ask_swap_msg_back(ct_swap_msg_back msg);

    //!
    //! 修改信息：
    //!     1.提供账号信息字段
    //!     2.服务器完成修改并下发    [服务器处理]
    //!
    bool ask_update_info(ct_ac_info info);

    //!
    //! 修改备注：
    //!     1.提供我方与好友账号，修改好友备注
    //!     2.服务器完成修改并下发    [服务器处理]
    //!
    bool ask_update_remarks(int64 account,int64 friends,string remarks);


    //!
    //! 获取信息：
    //!     1.提供账号获取所有信息
    //!     2.服务器查询并下发      [服务器处理]
    //!
    bool ask_info_all(int64 types,int64 account);


    //!
    //! 好友申请：
    //!     1.提供账号,好友账号,备注信息,提交到服务器
    //!     2.服务器转发到对方
    //!     3.对方接收并入库
    //!     4.对方确认或者拒绝,成功则对方提交到服务器
    //!     5.服务器写入双方好友列表
    //!     6.如果在线账号在线,服务器主动更新好友列表
    //!
    bool ask_swap_friens_add(ct_friend_add ct);
    bool ask_update_friends(int64 account,int64 friends,string ac_remarks,string fr_remarks);

    int64 is_online();
    sqlite_history* get_db();

signals:
    void sn_open();
    void sn_close();
    void sn_recv_msg(ct_swap_msg msg);

    void sn_friends_add();

    void sn_ac_login(int64 account,string nickname,string icon,vector<ct_friends_init> vec_friends);

    void sn_ac_info_all(int64 types,ct_ac_info info);
    void sn_ac_info_remarks(int64 friends,string remarks);

    void sn_update_info(string icon,bool ok);
    void sn_update_remarks(int64 friends,string remarks);
    void sn_update_friends(vector<ct_friends_init> vec);


//    void sn_ac_info(int64 account,string nickname,string icon);



//    void sn_ac_status(int64 ac_friends,string nickname,string icon,string remarks,bool online);
    void sn_ac_register(int64 account,string passwd,bool ok);

// int64 account,int64 phone,int64 age,int64 sex,string nickname,string location,string icon);





protected:

private:
    bool _is_online;        //是否连接
    inter_client _wc;       //网络链接
    sqlite_history _dbh;    //历史记录
    sqlite_record _dbr;     //申请记录

    std::map<string,function<void(const string&)>> _map_fn;

    void sl_open();
    void sl_message(const string &sjson);
    void sl_close();

    //发送接收信息
    void task_swap_msg(const string &sjson);
    void task_swap_msg_back(const string &sjson);

    //获取暂存信息，重新分别到任务处理函数
    void task_swap_cache_back(const string &sjson);


    //== 任务处理函数 ==
    void task_ac_login_back(const string &sjson);

    void task_ac_update_info_back(const string &sjson);
    void task_ac_update_remarks_back(const string &sjson);
    void task_ac_info_all_back(const string &sjson);

    void task_ac_register_back(const string &sjson);

    void task_ac_update_friends_back(const string &sjson);
    void task_swap_friens_add(const string &sjson);


//    void task_request_friens_back(const string &sjson);

//    void task_ac_info_back(const string &sjson);
//    void task_ac_info_remarks_back(const string &sjson);



//    void task_friends_list_back(const string &sjson);
//    void task_friends_status_back(const string &sjson);





    void task_error_info(const string &sjson);

    bool send_msg(const string &json);
};

////!
////! 类说明： 蜘蛛网络，单例模式，提供跨界连接网络
////!
//class qweb_client_spider : public QObject
//{
//    Q_OBJECT
//public:
//    void init_web(qweb_client *web) { _web = web; }
//    qweb_client* get_web() { return _web; }

//private:
//    qweb_client *_web = nullptr;
//};




#endif // QWEB_CLIENT_H
