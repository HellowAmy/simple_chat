#ifndef QWEB_CLIENT_H
#define QWEB_CLIENT_H

#include <QObject>
#include <QQueue>
#include <QMap>
#include <QIterator>
#include <functional>
#include <thread>
#include <condition_variable>

#include "include/hv/WebSocketClient.h"
#include "web_protocol.h"
#include "inter_client.h"


using namespace protocol;
using std::function;
using std::string;
using hv::WebSocketClient;

class qweb_client : public QObject
{
    Q_OBJECT
public:
    explicit qweb_client(QObject *parent = nullptr);
    ~qweb_client();

    int open(string ip = "127.0.0.1",int port = 4444,string txt = protocol::_head_);

    //!
    //! 登陆逻辑：
    //!     1.发送登陆请求
    //!     2.获取登陆反馈，发送好友列表请求
    //!     3.获取好友信息反馈，发送好友状态请求
    //!     4.获取好友状态反馈，完成登陆处理
    //!
    bool ask_login(int64 account,string passwd);

    inter_client* get_wc();

signals:
    void sn_open();
    void sn_close();

protected:

private:
    int64 _account;     //保存登陆账号
    inter_client _wc;   //网络链接

    QMap<string,function<void(const string&)>> _map_fn;

    void sl_open();
    void sl_message(const string &sjson);
    void sl_close();

    void task_login_back(const string &sjson);
    void task_friends_list_back(const string &sjson);
    void task_friends_status_back(const string &sjson);
    void task_error_info(const string &sjson);

    bool send_msg(const string &json);
};

#endif // QWEB_CLIENT_H
