#include "qweb_client.h"

#include "Tvlog.h"

#define ADD_MAP(func) _map_fn.emplace(func,std::bind(&qweb_client::task_##func,this,std::placeholders::_1))

qweb_client::qweb_client(QObject *parent)
    : QObject(parent)
{
    _wc.func_bind(std::bind(&qweb_client::sl_open,this),
              std::bind(&qweb_client::sl_message,this,std::placeholders::_1),
              std::bind(&qweb_client::sl_close,this));

    ADD_MAP(ac_login_back);
    ADD_MAP(ac_register_back);
    ADD_MAP(ac_info_back);
    ADD_MAP(ac_info_all_back);
    ADD_MAP(ac_update_info_back);
    ADD_MAP(friends_list_back);
    ADD_MAP(friends_status_back);
    ADD_MAP(swap_cache_back);
    ADD_MAP(swap_msg);
    ADD_MAP(swap_msg_back);
    ADD_MAP(error_info);

    _is_online = false;
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
    vlogi("sl_message: " << $(sjson));

    string stream;
    string type;
    if(check_json(sjson,stream,type))
    {
        if(stream == protocol::_sc_ || stream == protocol::_cc_)
        {
            auto it = _map_fn.find(type);
            if(it != _map_fn.end()) it->second(sjson);
        }
        else vlogw($(sjson));
    }
    else vlogw($(sjson));
}

void qweb_client::sl_close()
{
    _is_online = false;
    emit sn_close();
}

void qweb_client::task_ac_login_back(const string &sjson)
{
    int64 account;
    bool ok;
    if(get_ac_login_back(sjson,account,ok))
    {
        if(ok)
        {
            //打开数据库
            _db.open_history(account);

            //获取账号信息
            string s = set_ac_info(account);
            send_msg(s);
            _is_online = ok;
        }
        else vlogw("get_ac_login_back:" $(sjson));
    }
    else vlogw("get_ac_login_back:" $(sjson));

}

void qweb_client::task_ac_register_back(const string &sjson)
{
    int64 account;
    string passwd;
    bool ok;
    if(get_ac_register_back(sjson,account,passwd,ok))
    {
        emit sn_ac_register(account,passwd,ok);
    }
    else vlogw("task_ac_register_back:" $(sjson));
}

void qweb_client::task_ac_info_back(const string &sjson)
{
    int64 account;
    string nickname;
    string icon;
    bool ok;
    if(get_ac_info_back(sjson,account,nickname,icon,ok))
    {
        if(ok)
        {
            emit sn_ac_info(account,nickname,icon); //转发账号信息

            //获取好友列表
            string s = set_friends_list(account);
            send_msg(s);
        }
        else vlogw("task_ac_info_back:" $(sjson));
    }
    else vlogw("task_ac_info_back:" $(sjson));
}

void qweb_client::task_ac_info_all_back(const string &sjson)
{
    int64 account;
    int64 phone;
    int64 age;
    int64 sex;
    string nickname;
    string location;
    string icon;
    if(get_ac_info_all_back(sjson,account,phone,age,sex,nickname,location,icon))
    {
        emit sn_ac_info_all(account,phone,age,sex,nickname,location,icon);
    }
    else vlogw("task_ac_info_all_back:" $(sjson));
}

void qweb_client::task_ac_update_info_back(const string &sjson)
{
    int64 account;
    bool ok;
    if(get_ac_update_info_back(sjson,account,ok))
    {
        emit sn_update_info(ok);
    }
    else vlogw("task_friends_list_back:" $(sjson));
}

void qweb_client::task_friends_list_back(const string &sjson)
{
    int64 account;
    string svec_fs;
    bool ok;
    if(get_friends_list_back(sjson,account,svec_fs,ok))
    {
        if(ok)
        {
            string s = set_friends_status(account,svec_fs);
            send_msg(s);
        }
        else vlogw("task_friends_list_back:" $(sjson));
    }
    else vlogw("task_friends_list_back:" $(sjson));

}

void qweb_client::task_friends_status_back(const string &sjson)
{
    int64 account;
    string svec_ac_info;
    bool ok;
    if(get_friends_status_back(sjson,account,svec_ac_info,ok))
    {
        if(ok)
        {
            vector<string> vec = get_json_vec(svec_ac_info);
            for(auto a:vec)
            {
                int64 ac_friends;
                string nickname;
                string icon;
                bool online;
                if(get_ac_info_json(a,ac_friends,nickname,icon,online))
                {
                    //创建数据库
                    if(_db.create_history(ac_friends) == false)
                    { vlogw("create history failed" $(ac_friends)); }

                    //发送好友状态
                    emit sn_ac_status(ac_friends,nickname,icon,online);
                }
            }

            //请求暂存信息
            string s = set_swap_cache(account);
            send_msg(s);
        }
        else vlogw("get_friends_status_back:" $(sjson) $(ok));
    }
    else vlogw("get_friends_status_back:" $(sjson));
}

void qweb_client::task_swap_cache_back(const string &sjson)
{
    string svec_sjson;
    bool ok;
    if(get_swap_cache_back(sjson,svec_sjson,ok))
    {
        if(ok)
        {
            vector<string> vec = get_json_vec(svec_sjson);
            for(auto a:vec)
            {
                vloge("task_swap_cache_back: " <<$(a));
                sl_message(a);
            }
        }
        else vlogw("task_swap_cache_back:" $(sjson) $(ok));
    }
    else vlogw("task_swap_cache_back:" $(sjson));
}

void qweb_client::task_swap_msg(const string &sjson)
{
    int64 target;
    int64 source;
    int64 time_to;
    string type;
    string content;
    if(get_swap_msg(sjson,target,source,time_to,type,content))
    {
        vlogi($(target) $(source) $(time_to) $(type) $(content));

        //写入数据库
        if(_db.insert_history(source,{time_to,time_to,1,type,_object_AL_,content}) == false)
        { vlogw("insert_history failed"<<$(source) $(content) $(_db.get_error())); }

        //反馈接收信息
        emit sn_recv_msg(target,source,time_to,type,content);

        //接收确认反馈
        int64 time_ok = QDateTime::currentMSecsSinceEpoch();
        string s = set_swap_msg_back(source,target,time_to,time_ok,true);
        send_msg(s);
    }
    else vlogw("task_swap_msg:" $(sjson));
}

void qweb_client::task_swap_msg_back(const string &sjson)
{
    int64 target;
    int64 source;
    int64 time_to;
    int64 time_ok;
    bool ok;
    if(get_swap_msg_back(sjson,target,source,time_to,time_ok,ok))
    {
        vlogi($(target) $(source) $(time_to) $(time_ok) $(ok));
        if(ok)
        {
            //写入数据库确认反馈
            if(_db.update_feed_back(source,time_to,time_ok) == false)
            { vlogw("swap_msg_back: update_feed_back failed"); }
        }
        else vlogw("task_swap_msg:" $(sjson) $(ok));
    }
    else vlogw("task_swap_msg:" $(sjson));
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
    else vlogw("task_error_info:" $(sjson));
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

void qweb_client::close()
{
    _wc.get_wc()->close();
}

bool qweb_client::ask_login(int64 account, string passwd)
{
    string sjson = set_ac_login(account,passwd);
    return send_msg(sjson);
}

bool qweb_client::ask_register(int64 phone, int64 age, int64 sex, string nickname, string location, string passwd)
{
    string sjson = set_ac_register(phone,age,sex,nickname,location,passwd);
    return send_msg(sjson);
}

bool qweb_client::ask_swap_msg(int64 target,int64 source, int64 time_to, string type, string content)
{
    //插入历史数据库
    bool ok = _db.insert_history(target,{time_to,0,0,type,_object_AR_,content});
    if(ok)
    {
        //发送信息
        string sjson = set_swap_msg(target,source,time_to,type,content);
        return send_msg(sjson);
    }
    else vlogw("insert_history failed" $(target) $(source) $(_db.get_error_exec()));
    return false;
}

bool qweb_client::ask_swap_msg_back(int64 target, int64 source, int64 time_to, int64 time_ok)
{
    string sjson = set_swap_msg_back(target,source,time_to,time_ok,true);
    return send_msg(sjson);
}

bool qweb_client::ask_update_info(int64 account, int64 phone, int64 age, int64 sex, string nickname, string location,string icon)
{
    string sjson = set_ac_update_info(account,phone,age,sex,nickname,location,icon);
    return send_msg(sjson);
}

int64 qweb_client::is_online()
{
    return _is_online;
}

sqlite_history *qweb_client::get_db()
{
    return &_db;
}

