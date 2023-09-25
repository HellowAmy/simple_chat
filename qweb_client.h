#ifndef QWEB_CLIENT_H
#define QWEB_CLIENT_H

#include <QObject>
#include <QQueue>
#include <functional>
#include <thread>
#include <condition_variable>

#include "include/hv/WebSocketClient.h"
#include "web_protocol.h"


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

    int ask_login(int64 account,string passwd);

    WebSocketClient* get_wc();

signals:
    void sn_open();
    void sn_message(const string &msg);
    void sn_close();

protected:

private:
    //网络链接
    WebSocketClient _wc;
    reconn_setting_t _rec;

    void func_bind(function<void()> open,
                   function<void(const string& message)> message,
                   function<void()> close);


};

#endif // QWEB_CLIENT_H
