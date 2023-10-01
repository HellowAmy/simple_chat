#include "qweb_files.h"

#define ADD_MAP(func) _map_fn.insert(func,std::bind(&qweb_files::task_##func,this,std::placeholders::_1))


qweb_files::qweb_files(QObject *parent)
    : QObject(parent)
{
    _wc.func_bind(std::bind(&qweb_files::sl_open,this),
                  std::bind(&qweb_files::sl_message,this,std::placeholders::_1),
                  std::bind(&qweb_files::sl_close,this));
}

int qweb_files::open(string ip, int port, string txt)
{
    return _wc.open(ip,port,txt);
}

bool qweb_files::send_msg(const string &sjson)
{
    _wc.get_wc()->send(sjson);
    return _wc.get_wc()->isConnected();
}

void qweb_files::sl_open()
{
    emit sn_open();
}

void qweb_files::sl_message(const string &sjson)
{

}

void qweb_files::sl_close()
{
    emit sn_close();
}
