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
    //!     1.发送登陆请求,获取登陆反馈
    //!     2.请求账号信息，获取账号信息反馈,信号转发账号信息
    //!     3.发送好友列表请求，获取好友信息反馈
    //!     4.发送好友状态请求，获取好友状态反馈，信号转发好友状态
    //!     5.发送请求暂存信息，获取暂存信息反馈，信号转发暂存信息，完成登陆处理
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
    //!     1.发送信息，完成交换处理
    //!     2.服务器转发信息       [服务器处理]
    //!     3.对方账号接收信息      [对方处理]
    bool ask_swap_msg(int64 target,int64 source,int64 time_to,string type,string content);
    bool ask_swap_msg_back(int64 target,int64 source,int64 time_to,int64 time_ok);

    //!
    //! 修改信息：
    //!     1.提供账号信息字段
    //!     2.服务器完成修改并下发    [服务器处理]
    //!
    bool ask_update_info(int64 account,int64 phone,int64 age,int64 sex,string nickname,string location,string icon);

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
    bool ask_info_all(int64 account);

    //!
    //! 获取备注：
    //!     1.提供我方与好友账号
    //!     2.服务器查询并下发      [服务器处理]
    //!
    bool ask_info_remarks(int64 account,int64 friends);



    int64 is_online();
    sqlite_history* get_db();

signals:
    void sn_open();
    void sn_close();
    void sn_ac_info(int64 account,string nickname,string icon);
    void sn_ac_info_remarks(int64 friends,string remarks);
    void sn_ac_status(int64 ac_friends,string nickname,string icon,string remarks,bool online);
    void sn_ac_register(int64 account,string passwd,bool ok);
    void sn_ac_info_all(int64 account,int64 phone,int64 age,int64 sex,string nickname,string location,string icon);
    void sn_update_info(bool ok);
    void sn_update_remarks(int64 friends,bool ok);
    void sn_recv_msg(int64 target, int64 source,int64 time_to, string type, string content);


protected:

private:
    bool _is_online;        //是否连接
    inter_client _wc;       //网络链接
    sqlite_history _db;     //历史记录

    std::map<string,function<void(const string&)>> _map_fn;

    void sl_open();
    void sl_message(const string &sjson);
    void sl_close();

    void task_ac_login_back(const string &sjson);
    void task_ac_register_back(const string &sjson);
    void task_ac_info_back(const string &sjson);
    void task_ac_info_remarks_back(const string &sjson);
    void task_ac_info_all_back(const string &sjson);
    void task_ac_update_info_back(const string &sjson);
    void task_ac_update_remarks_back(const string &sjson);
    void task_friends_list_back(const string &sjson);
    void task_friends_status_back(const string &sjson);
    void task_swap_cache_back(const string &sjson);
    void task_swap_msg(const string &sjson);
    void task_swap_msg_back(const string &sjson);
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
