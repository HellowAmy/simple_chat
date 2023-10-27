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
    ADD_MAP(ac_info_all_back);

    ADD_MAP(ac_update_info_back);
    ADD_MAP(ac_update_remarks_back);
    ADD_MAP(ac_update_friends_back);

    ADD_MAP(swap_friens_add);
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
    string svec_friends_info;
    string nickname;
    string icon;
    if (get_ac_login_back(sjson,account,nickname,icon,svec_friends_info))
    {
        //打开数据库-历史记录
        if(_dbh.open_history(account) == false)
        { vlogw("open history failed" $(account)); }

        //打开数据库-好友申请
        if(_dbr.open_record(account) == false)
        { vlogw("open record failed" $(account)); }

        //加入好友信息并反馈
        vector<ct_friends_init> vec_friends;
        vector<string> vec = get_json_vec(svec_friends_info);
        for(const auto &a:vec)
        {
            ct_friends_init ct;
            if(get_friends_info_json(a,ct.friends,ct.nickname,ct.icon,ct.remarks,ct.online))
            {
                //创建数据库
                if(_dbh.create_history(ct.friends) == false)
                { vlogw("create history failed" $(ct.friends)); }

                vec_friends.push_back(ct);
            }
        }
        emit sn_ac_login(account,nickname,icon,vec_friends);

        //请求暂存信息
        string s = set_swap_cache(account);
        send_msg(s);
    }
    else vlogw("ac_login_back:" $(sjson));


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

void qweb_client::task_swap_friens_add(const string &sjson)
{
    int64 time;
    int64 account;
    int64 friends;
    string notes;
    string remarks;
    if(get_swap_friens_add(sjson,time,account,friends,notes,remarks))
    {
        if(_dbr.insert_record(friends,time,account,notes,remarks))
        { emit sn_friends_add(); }
        else { vlogw("insert_record failed"); }
    }
}

void qweb_client::task_ac_update_friends_back(const string &sjson)
{
    string svec_friends_info;
    if(get_ac_update_friends_back(sjson,svec_friends_info))
    {
        //加入好友信息并反馈
        vector<ct_friends_init> vec_friends;
        vector<string> vec = get_json_vec(svec_friends_info);
        for(const auto &a:vec)
        {
            ct_friends_init ct;
            if(get_friends_info_json(a,ct.friends,ct.nickname,ct.icon,ct.remarks,ct.online))
            { vec_friends.push_back(ct); }
        }
        emit sn_update_friends(vec_friends);
    }
}


void qweb_client::task_ac_info_all_back(const string &sjson)
{
    int64 types;
    int64 account;
    int64 phone;
    int64 age;
    int64 sex;
    string nickname;
    string location;
    string icon;
    if(get_ac_info_all_back(sjson,types,account,phone,age,sex,nickname,location,icon))
    {
        ct_ac_info info{account,phone,age,sex,nickname,location,icon};
        emit sn_ac_info_all(types,info);
    }
    else vlogw("task_ac_info_all_back:" $(sjson));
}

void qweb_client::task_ac_update_info_back(const string &sjson)
{
    int64 account;
    string icon;
    bool ok;
    if(get_ac_update_info_back(sjson,account,icon,ok))
    {
        emit sn_update_info(icon,ok);
    }
    else vlogw("task_friends_list_back:" $(sjson));
}

void qweb_client::task_ac_update_remarks_back(const string &sjson)
{
    int64 account;
    int64 friends;
    string remarks;
    if(get_ac_update_remarks_back(sjson,account,friends,remarks))
    {
        emit sn_update_remarks(friends,remarks);
    }
    else vlogw("task_friends_list_back:" $(sjson));
}

void qweb_client::task_swap_cache_back(const string &sjson)
{
    string svec_sjson;
    if(get_swap_cache_back(sjson,svec_sjson))
    {
        vector<string> vec = get_json_vec(svec_sjson);
        for(auto a:vec)
        { sl_message(a); }
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
        if(_dbh.insert_history(source,{time_to,time_to,1,type,_object_AL_,content}) == false)
        { vlogw("insert_history failed"<<$(source) $(content) $(_dbh.get_error())); }

        //反馈接收信息
        ct_swap_msg ct{target,source,time_to,type,content};
        emit sn_recv_msg(ct);

        //接收确认反馈
        int64 time_ok = QDateTime::currentMSecsSinceEpoch();
        string s = set_swap_msg_back(source,target,time_to,time_ok);
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
    if(get_swap_msg_back(sjson,target,source,time_to,time_ok))
    {
        //写入数据库确认反馈
        if(_dbh.update_feed_back(source,time_to,time_ok) == false)
        { vlogw("swap_msg_back: update_feed_back failed"); }
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

bool qweb_client::ask_swap_msg(ct_swap_msg msg)
{
    //插入历史数据库
    bool ok = _dbh.insert_history(msg.target,{msg.time_to,0,0,msg.types,_object_AR_,msg.content});
    if(ok)
    {
        //发送信息
        string sjson = set_swap_msg(msg.target,msg.source,msg.time_to,msg.types,msg.content);
        return send_msg(sjson);
    }
    else vlogw("insert_history failed" $(msg.target) $(msg.source) $(_dbh.get_error_exec()));
    return false;
}

bool qweb_client::ask_swap_msg_back(ct_swap_msg_back msg)
{
    string sjson = set_swap_msg_back(msg.target,msg.source,msg.time_to,msg.time_ok);
    return send_msg(sjson);
}

bool qweb_client::ask_update_info(ct_ac_info info)
{
    string sjson = set_ac_update_info(info.account,info.phone,info.age,info.sex,
                                      info.nickname,info.location,info.icon);
    return send_msg(sjson);
}

bool qweb_client::ask_update_remarks(int64 account, int64 friends, string remarks)
{
    string sjson = set_ac_update_remarks(account,friends,remarks);
    return send_msg(sjson);
}

bool qweb_client::ask_info_all(int64 types,int64 account)
{
    string sjson = set_ac_info_all(types,account);
    return send_msg(sjson);
}

bool qweb_client::ask_swap_friens_add(ct_friend_add ct)
{
    string sjson = set_swap_friens_add(ct.time,ct.account,ct.friends,ct.notes,ct.remarks);
    return send_msg(sjson);
}

bool qweb_client::ask_update_friends(int64 account, int64 friends, string ac_remarks, string fr_remarks)
{
    string sjson = set_ac_update_friends(account,friends,ac_remarks,fr_remarks);
    return send_msg(sjson);
}

int64 qweb_client::is_online()
{
    return _is_online;
}

sqlite_history *qweb_client::get_db()
{
    return &_dbh;
}

