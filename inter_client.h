#ifndef INTER_CLIENT_H
#define INTER_CLIENT_H

#include <functional>
#include <string>
#include "include/hv/WebSocketClient.h"

using namespace protocol;
using std::function;
using std::string;
using hv::WebSocketClient;

//!
//! 类说明： WebSocket 客户端连接
//!
class inter_client
{
public:
    inter_client() {}
    ~inter_client() { if(_wc.isConnected()) _wc.close(); }

    void func_bind(function<void()> open,
                   function<void(const string& message)> message,
                   function<void()> close)
    {
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

        string temp = "ws://"+ip+":"+hv::to_string(port)+"/"+txt;
        return _wc.open(temp.c_str());
    }

    WebSocketClient* get_wc(){ return &_wc; }

protected:
    WebSocketClient _wc;
    reconn_setting_t _rec;
};

#endif // INTER_CLIENT_H
