#ifndef QWEB_SOCKET_H
#define QWEB_SOCKET_H

#include <QObject>
#include <functional>

//#include "include/hv/WebSocket.h"
#include "include/hv/WebSocketServer.h"
#include "include/hv/WebSocketClient.h"

using std::function;
using std::string;

//using hv::WebSocketChannelPtr;
//using hv::HttpRequestPtr;
using hv::WebSocketServer;
using hv::WebSocketService;
using hv::WebSocketClient;
//using namespace hv;

typedef WebSocketChannelPtr sp_channe;
typedef HttpRequestPtr sp_http;

//!
//! 类说明： WebSocket 服务器连接
//!
class qweb_server : public QObject
{
    Q_OBJECT
public:
    explicit qweb_server(QObject *parent = nullptr) : QObject(parent) {}

    void func_bind(function<void(const sp_channe&, const sp_http&)> open,
                   function<void(const sp_channe&, const string&)> message,
                   function<void(const sp_channe&)> close)
    {
        _wsc.onopen = open;
        _wsc.onmessage = message;
        _wsc.onclose = close;
    }

    int open(int port = 4444,int th_mun = 4)
    {
        _wsv.registerWebSocketService(&_wsc);
        _wsv.setPort(port);
        _wsv.setThreadNum(th_mun);
        return _wsv.run();
    }


    WebSocketServer* get_sv() { return &_wsv; }
    WebSocketService* get_sc() { return &_wsc; }

signals:

protected:
    WebSocketServer _wsv;
    WebSocketService _wsc;
};



//!
//! 类说明： WebSocket 客户端连接
//!
class qweb_client : public QObject
{
    Q_OBJECT
public:
    explicit qweb_client(QObject *parent = nullptr) : QObject(parent) {}

//    void func_bind(function<void()> open,
//                   function<void(const string& message)> message,
//                   function<void()> close)
//    {
//        _wc.onopen = open;
//        _wc.onmessage = message;
//        _wc.onclose = close;
//    }

    int open(string ip = "127.0.0.1",int port = 4444,string txt = "web_test/new/pos")
    {
        reconn_setting_init(&_rec);
        _rec.min_delay = 1000;
        _rec.max_delay = 2000;
        _rec.delay_policy = 1;
        _wc.setReconnect(&_rec);

        string temp = "ws://"+ip+":"+hv::to_string(port)+"/"+txt;
        return _wc.open(temp.c_str());
    }

    void close_client() { _wc.closesocket(); _wc.close(); }
    WebSocketClient* get_wc() {  return &_wc; }

protected:
    WebSocketClient _wc;
    reconn_setting_t _rec;
};

#endif // QWEB_SOCKET_H


/*

typedef WebSocketChannelPtr web_sock;
typedef HttpRequestPtr web_http;

class inter_server
{
public:
    void func_bind(function<void(const web_sock&, const web_http&)> open,
                   function<void(const web_sock&, const string&)> message,
                   function<void(const web_sock&)> close)
    {
        _wsc.onopen = open;
        _wsc.onmessage = message;
        _wsc.onclose = close;
    }

    WebSocketService& sock() { return _wsc; }
    WebSocketServer& server() { return _wsv; }

    int open(int port = 4444,int th_count = 4)
    {
        _wsv.registerWebSocketService(&_wsc);
        _wsv.setPort(port);
        _wsv.setThreadNum(th_count);
        return _wsv.run();
    }

    void stop_server(){ _wsv.stop(); };
    void start_server(){ _wsv.start(); };

protected:
    WebSocketServer _wsv;
    WebSocketService _wsc;
};

class inter_client
{
public:
    WebSocketClient& sock() { return _wc; }

    void func_bind(function<void()> open,
                   function<void(const string& msg)> message,
                   function<void()> close){
        _wc.onopen = open;
        _wc.onmessage = message;
        _wc.onclose = close;
    }

    int open(string ip = "127.0.0.1",int port = 4444,string txt = "")
    {
        reconn_setting_init(&_rec);
        _rec.min_delay = 1000;
        _rec.max_delay = 2000;
        _rec.delay_policy = 1;
        _wc.setReconnect(&_rec);

        string temp = "ws://"+ip+":"+to_string(port)+"/"+txt;
        return _wc.open(temp.c_str());
    }

    void close_client()
    { _wc.closesocket(); _wc.close(); }

protected:
    WebSocketClient _wc;
    reconn_setting_t _rec;
};

*/
