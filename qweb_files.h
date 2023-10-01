#ifndef QWEB_FILES_H
#define QWEB_FILES_H

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

class qweb_files : public QObject
{
    Q_OBJECT
public:
    explicit qweb_files(QObject *parent = nullptr);

    int open(string ip = "127.0.0.1",int port = 4444,string txt = protocol::_head_);

signals:
    void sn_open();
    void sn_close();

protected:
    inter_client _wc;   //网络链接

    QMap<string,function<void(const string&)>> _map_fn;

    void sl_open();
    void sl_message(const string &sjson);
    void sl_close();

    bool send_msg(const string &json);
};

#endif // QWEB_FILES_H
