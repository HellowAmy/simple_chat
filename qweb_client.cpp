#include "qweb_client.h"

//#include "../../Tvlog.h"

qweb_client::qweb_client(QObject *parent)
    : QObject(parent)
{
    func_bind(std::bind(&qweb_client::sn_open,this),
              std::bind(&qweb_client::sl_message,this,std::placeholders::_1),
              std::bind(&qweb_client::sn_close,this));
}

void qweb_client::func_bind(function<void()> open,
               function<void(const string& message)> message,
               function<void()> close)
{
    _wc.onopen = open;
    _wc.onmessage = message;
    _wc.onclose = close;
}

void qweb_client::sl_message(const string &message)
{
    emit sn_message(QString::fromStdString(message));
}

int qweb_client::open(string ip,int port,string txt)
{
    reconn_setting_init(&_rec);
    _rec.min_delay = 1000;
    _rec.max_delay = 2000;
    _rec.delay_policy = 1;
    _wc.setReconnect(&_rec);

    string temp = "ws://"+ip+":"+hv::to_string(port)+"/"+txt;
    return _wc.open(temp.c_str());
}

WebSocketClient* qweb_client::get_wc()
{
    return &_wc;
}
