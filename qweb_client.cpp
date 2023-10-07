#include "qweb_client.h"

#include "Tvlog.h"

#define ADD_MAP(func) _map_fn.insert(func,std::bind(&qweb_client::task_##func,this,std::placeholders::_1))

qweb_client::qweb_client(QObject *parent)
    : QObject(parent)
{
    _wc.func_bind(std::bind(&qweb_client::sl_open,this),
              std::bind(&qweb_client::sl_message,this,std::placeholders::_1),
              std::bind(&qweb_client::sl_close,this));

    ADD_MAP(login_back);
    ADD_MAP(friends_list_back);
    ADD_MAP(friends_status_back);
    ADD_MAP(error_info);
}

qweb_client::~qweb_client()
{

}

void qweb_client::sl_open()
{
    emit sn_open();
}

void qweb_client::sl_message(const string &sjson)
{
//    vlogi("sl_message: " << $(sjson));

    string stream;
    string type;
    if(check_json(sjson,stream,type))
    {
        if(stream == protocol::_sc_)
        {
            auto it = _map_fn.value(type,nullptr);
            if(it != nullptr) it(sjson);
        }
        else if(stream == protocol::_cc_)
        {

        }
        else vlogw($(sjson));
    }
    else vlogw($(sjson));
}

void qweb_client::sl_close()
{
    emit sn_close();
}

void qweb_client::task_login_back(const string &sjson)
{
    bool ok_send = false;
    bool ok;
    if(get_login_back(sjson,ok))
    {
        if(ok)
        {
            string s = set_friends_list(_account);
            send_msg(s);
            ok_send = true;
        }
    }

    vlogif(ok_send,$(ok_send) $(sjson));
}

void qweb_client::task_friends_list_back(const string &sjson)
{
    bool ok_send = false;
    bool ok;
    string svec_fs;
    if(get_friends_list_back(sjson,svec_fs,ok))
    {
        if(ok)
        {
            string s = set_friends_status(svec_fs);
            send_msg(s);
            ok_send = true;
        }
    }

    vlogif(ok_send,$(ok_send) $(sjson));
}

void qweb_client::task_friends_status_back(const string &sjson)
{
    bool ok_send = false;
    string svec_ac_info;
    bool ok;
    if(get_friends_status_back(sjson,svec_ac_info,ok))
    {
        if(ok)
        {
            vector<string> vec_s = get_json_vec(svec_ac_info);
            for(auto a:vec_s)
            {
                int64 ac_friends;
                string nickname;
                string icon;
                bool online;
                if(get_ac_info_json(a,ac_friends,nickname,icon,online))
                {
                    emit sn_ac_status(ac_friends,nickname,icon,online);
                    vlogi($(ac_friends) $(nickname) $(icon) $(online));
                }
            }
            ok_send = true;
        }
    }

    vlogif(ok_send,$(ok_send) $(sjson));
}

void qweb_client::task_error_info(const string &sjson)
{
    bool ok_send = false;
    int error;
    string info;
    if(get_error_info(sjson,error,info))
    {
        ok_send = true;
        vlogi($(error) $(info));
    }

    vlogif(ok_send,$(ok_send) $(sjson));
}

bool qweb_client::send_msg(const string &sjson)
{
    _wc.get_wc()->send(sjson);
    return _wc.get_wc()->isConnected();
}

int qweb_client::open(string ip,int port,string txt)
{
    return _wc.open(ip,port,txt);
}

bool qweb_client::ask_login(int64 account, string passwd)
{
    _account = account;
    string sjson = set_login(account,passwd);
    return send_msg(sjson);
}

inter_client* qweb_client::get_wc()
{
    return &_wc;
}

