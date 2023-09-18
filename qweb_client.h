#ifndef QWEB_CLIENT_H
#define QWEB_CLIENT_H

#include <QObject>
#include <functional>

#include "include/hv/WebSocketClient.h"

using std::function;
using std::string;
using hv::WebSocketClient;

class qweb_client : public QObject
{
    Q_OBJECT
public:
    explicit qweb_client(QObject *parent = nullptr);

    int open(string ip = "127.0.0.1",int port = 4444,string txt = "web_test/new/pos");

    WebSocketClient* get_wc();

signals:
    void sn_open();
    void sn_message(QString msg);
    void sn_close();

protected:

private:
    WebSocketClient _wc;
    reconn_setting_t _rec;

    void func_bind(function<void()> open,
                   function<void(const string& message)> message,
                   function<void()> close);

    void sl_message(const string& message);
};

#endif // QWEB_CLIENT_H
