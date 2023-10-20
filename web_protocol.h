#ifndef WEB_PROTOCOL_H
#define WEB_PROTOCOL_H

#include "include/nlohmann/json.hpp"

#include <string>
#include <vector>
#include <algorithm>

typedef long long int64;
typedef nlohmann::json json;
using std::string;
using std::vector;

#define CS_SERVER_ADDRESS "127.0.0.1"
#define CS_PORT_TASKS 4444
#define CS_PORT_FILES 4433


//!
//! head        :""  协议,判断是否为本程序协议
//! version     :""  版本,兼容协议的修改
//! stream      :""  流向,判断发送方意图，    [cc：客户端转发,sc：服务器下发,cs：客户端请求]
//! type        :""  类型,任务具体类型，     [login,swap,logup，...]
//! extend      :{}  扩展,携带各类协议具体信息
//!
namespace protocol {

static string _sjson_ = R"(
        {
            "head"      :"",
            "version"   :"",
            "stream"    :"",
            "type"      :"",
            "extend"    :{}
        } )";
static string _head_       = "SMCHPRO";
static string _version_    = "1";
static string _cc_         = "cc";
static string _sc_         = "sc";
static string _cs_         = "cs";



static string make_json(const string &stream,
                        const string &type,
                        const string &sjson     = _sjson_,
                        const string &head      = _head_,
                        const string &version   = _version_)
{
    json js = json::parse(sjson);
    js["head"]      = head;
    js["version"]   = version;
    js["stream"]    = stream;
    js["type"]      = type;
    return js.dump();
}

static bool check_head(const json &js, const string &head = _head_,
                       const string &version = _version_)
{
    if(js["head"] == head && js["version"] == version) return true;
    return false;
}

static bool check_json(const string &sjson,string &stream,string &type)
{
    bool ret = false;
    try {
        json js = json::parse(sjson);
        stream = js["stream"].get<string>();
        type = js["type"].get<string>();
        ret = check_head(js);
    } catch(...){}
    return ret;
}

static bool check_swap(const string &sjson,int64 &target,int64 &source)
{
    bool ret = false;
    try {
        json js = json::parse(sjson);
        target = js["extend"]["target"].get<int64>();
        source = js["extend"]["source"].get<int64>();
        ret = true;
    } catch(...){}
    return ret;
}

static bool vec_stoi(const vector<string> &vec_str,vector<int64> &vec_int)
{
    bool ret = true;
    try {
        for(const auto &a:vec_str)
        { vec_int.push_back(std::stoll(a)); }
    } catch(...) { ret = false; }
    return ret;
}

template<class T>
static void set_extend(json &js,const string &index,T value)
{
    js["extend"][index] = value;
}

template<class T>
static T get_extend(json &js,const string &index)
{
    json::object_t tm = js["extend"];
    T name = tm[index];
    return name;
}

static string set_json_vec(const vector<string> &vec)
{
    string str;
    try {
        json js;
        js["vec"] = vec;
        str = js.dump();
    } catch(...){}
    return str;
}

static vector<string> get_json_vec(const string &sjson)
{
    vector<string> vec;
    try {
        json js = json::parse(sjson);
        vec = js["vec"].get<vector<string>>();
    } catch(...){}
    return vec;
}

static string set_friends_info_json(int64 friends,const string &nickname,const string &icon,const string &remarks,bool online)
{
    string str;
    try {
        json js;
        js["friends"]    = friends;
        js["nickname"]      = nickname;
        js["icon"]          = icon;
        js["remarks"]       = remarks;
        js["online"]        = online;
        str = js.dump();
    } catch(...){}
    return str;
}

static bool get_friends_info_json(const string &sjson,int64 &friends,string &nickname,string &icon,string &remarks,bool &online)
{
    bool ret = false;
    try {
        json js = json::parse(sjson);
        friends     = js["friends"];
        nickname    = js["nickname"];
        icon        = js["icon"];
        remarks     = js["remarks"];
        online      = js["online"];
        ret = true;
    } catch(...){}
    return ret;
}





#define CS_1(in_type,in_value) in_type in_value
#define CS_2(in_type,in_value) set_extend<in_type>(js,#in_value,in_value);
#define CS_3(in_type,in_value) in_type &in_value
#define CS_4(in_type,in_value) in_value = get_extend<in_type>(js,#in_value);

#define CS_ARGV(...) __VA_ARGS__
#define CS_BODY(...) __VA_ARGS__
#define CS_MAKE_TYPE(in_type,in_stream,in_sarg,in_sbody,in_garg,in_gbody)       \
    static string in_type = #in_type;                                           \
    static string set_##in_type(in_sarg)                                        \
    {                                                                           \
        string tm_sjson = make_json(in_stream,in_type);                         \
        json js = json::parse(tm_sjson);                                        \
        in_sbody                                                                \
        return js.dump();                                                       \
    }                                                                           \
    static bool get_##in_type(const string &tm_sjson in_garg)                   \
    {                                                                           \
        json js = json::parse(tm_sjson);                                        \
        try{ in_gbody return true; }                                            \
        catch(...) { return false; }                                            \
    }



#define CS_ERR_PARSE_JSON       1
#define CS_ERR_NOT_TASK         2
#define CS_ERR_NOT_STREAM       3
#define CS_ERR_SWAP_SJSON       4
#define CS_ERR_SELECT_DATA      5
#define CS_LOGIN_NOT_AC         10
#define CS_LOGIN_PASSWD_ERR     11
#define CS_SELECT_ERR           20


#define CS_ERR_PARSE_JSON_S         ""
#define CS_ERR_NOT_TASK_S           "not find task function from transmit"
#define CS_ERR_NOT_STREAM_S         "not find stream from transmit"
#define CS_ERR_SWAP_SJSON_S         "swap error form add cache failed"
#define CS_ERR_SELECT_DATA_S        "select data error from server"

#define CS_LOGIN_NOT_AC_S           "login account not exist"
#define CS_LOGIN_PASSWD_ERR_S       "login password is error"

//!
//! error_info : 错误反馈
//!
//! extend :
//!     int error       错误代码
//!     string info     错误信息
//!
CS_MAKE_TYPE(error_info,_sc_,
             CS_ARGV( CS_1(int,error),CS_1(string,info)  ),
             CS_BODY( CS_2(int,error) CS_2(string,info)  ),
             CS_ARGV(,CS_3(int,error),CS_3(string,info)  ),
             CS_BODY( CS_4(int,error) CS_4(string,info)  )
             )

//!
//! online_status : 上线状态
//!
//! extend :
//!     uint account    好友账号
//!     bool online     在线状态
//!
CS_MAKE_TYPE(online_status,_sc_,
             CS_ARGV( CS_1(int64,ac_friend),CS_1(bool,online)  ),
             CS_BODY( CS_2(int64,ac_friend) CS_2(bool,online)  ),
             CS_ARGV(,CS_3(int64,ac_friend),CS_3(bool,online)  ),
             CS_BODY( CS_4(int64,ac_friend) CS_4(bool,online)  )
             )


//!
//! ac_login : 登陆账号
//!
//! extend :
//!     uint account    账号
//!     string passwd   密码
//!
//! back :
//!     uint account    账号
//!     string svec_friends_info    好友列表的数组
//!     string nickname             昵称
//!     string icon                 头像
//!
//!     | svec_friends_info 存储json格式： 使用 set_friends_info_json 和 get_friends_info_json 函数解析 )
//!     | uint friends      好友账号
//!     | string nickname   昵称
//!     | string icon       头像
//!     | string remarks    备注
//!     | bool online       在线
//!
CS_MAKE_TYPE(ac_login,_cs_,
             CS_ARGV( CS_1(int64,account),CS_1(string,passwd)  ),
             CS_BODY( CS_2(int64,account) CS_2(string,passwd)  ),
             CS_ARGV(,CS_3(int64,account),CS_3(string,passwd)  ),
             CS_BODY( CS_4(int64,account) CS_4(string,passwd)  )
             )
CS_MAKE_TYPE(ac_login_back,_sc_,
             CS_ARGV( CS_1(int64,account),CS_1(string,nickname),CS_1(string,icon),CS_1(string,svec_friends_info)  ),
             CS_BODY( CS_2(int64,account) CS_2(string,nickname) CS_2(string,icon) CS_2(string,svec_friends_info)  ),
             CS_ARGV(,CS_3(int64,account),CS_3(string,nickname),CS_3(string,icon),CS_3(string,svec_friends_info)  ),
             CS_BODY( CS_4(int64,account) CS_4(string,nickname) CS_4(string,icon) CS_4(string,svec_friends_info)  )
             )


//!
//! ac_register : 登陆注册
//!
//! extend :
//!     uint phone          账号
//!     uint age            年龄
//!     uint sex            性别
//!     string nickname     昵称
//!     string location     地址
//!     string icon         头像
//!     string passwd       密码
//!
//! back :
//!     uint account        账号
//!     string passwd       密码
//!     bool ok             确认
//!
CS_MAKE_TYPE(ac_register,_cs_,
             CS_ARGV( CS_1(int64,phone),CS_1(int64,age),CS_1(int64,sex),CS_1(string,nickname),CS_1(string,location),CS_1(string,passwd)  ),
             CS_BODY( CS_2(int64,phone) CS_2(int64,age) CS_2(int64,sex) CS_2(string,nickname) CS_2(string,location) CS_2(string,passwd)  ),
             CS_ARGV(,CS_3(int64,phone),CS_3(int64,age),CS_3(int64,sex),CS_3(string,nickname),CS_3(string,location),CS_3(string,passwd)  ),
             CS_BODY( CS_4(int64,phone) CS_4(int64,age) CS_4(int64,sex) CS_4(string,nickname) CS_4(string,location) CS_4(string,passwd)  )
             )
CS_MAKE_TYPE(ac_register_back,_sc_,
             CS_ARGV( CS_1(int64,account),CS_1(string,passwd),CS_1(bool,ok)  ),
             CS_BODY( CS_2(int64,account) CS_2(string,passwd) CS_2(bool,ok)  ),
             CS_ARGV(,CS_3(int64,account),CS_3(string,passwd),CS_3(bool,ok)  ),
             CS_BODY( CS_4(int64,account) CS_4(string,passwd) CS_4(bool,ok)  )
             )
//
////!
////! ac_info : 账号信息
////!
////! extend :
////!     uint account        账号
////!
////! back :
////!     string nickname     昵称
////!     string icon         头像
////!     bool ok             确认
////!
//CS_MAKE_TYPE(ac_info,_cs_,
//             CS_ARGV( CS_1(int64,account)  ),
//             CS_BODY( CS_2(int64,account)  ),
//             CS_ARGV(,CS_3(int64,account)  ),
//             CS_BODY( CS_4(int64,account)  )
//             )
//CS_MAKE_TYPE(ac_info_back,_sc_,
//             CS_ARGV( CS_1(int64,account),CS_1(string,nickname),CS_1(string,icon),CS_1(bool,ok)  ),
//             CS_BODY( CS_2(int64,account) CS_2(string,nickname) CS_2(string,icon) CS_2(bool,ok)  ),
//             CS_ARGV(,CS_3(int64,account),CS_3(string,nickname),CS_3(string,icon),CS_3(bool,ok)  ),
//             CS_BODY( CS_4(int64,account) CS_4(string,nickname) CS_4(string,icon) CS_4(bool,ok)  )
//             )

////!
////! ac_info_remarks : 好友账号备注
////!
////! extend :
////!     uint account        账号
////!     uint friends        好友账号
////!
////! back :
////!     uint friends        昵称
////!     string remarks      备注
////!
//CS_MAKE_TYPE(ac_info_remarks,_cs_,
//             CS_ARGV( CS_1(int64,account),CS_1(int64,friends)  ),
//             CS_BODY( CS_2(int64,account) CS_2(int64,friends)  ),
//             CS_ARGV(,CS_3(int64,account),CS_3(int64,friends)  ),
//             CS_BODY( CS_4(int64,account) CS_4(int64,friends)  )
//             )
//CS_MAKE_TYPE(ac_info_remarks_back,_sc_,
//             CS_ARGV( CS_1(int64,friends),CS_1(string,remarks)  ),
//             CS_BODY( CS_2(int64,friends) CS_2(string,remarks)  ),
//             CS_ARGV(,CS_3(int64,friends),CS_3(string,remarks)  ),
//             CS_BODY( CS_4(int64,friends) CS_4(string,remarks)  )
//             )

//!
//! ac_info_all : 账号信息--所有
//!
//! extend :
//!     uint account        账号
//!
//! back :
//!     uint account        账号
//!     uint phone          电话
//!     uint age            年龄
//!     uint sex            性别
//!     string nickname     昵称
//!     string location     地址
//!     string icon         头像
//!
CS_MAKE_TYPE(ac_info_all,_cs_,
             CS_ARGV( CS_1(int64,account)  ),
             CS_BODY( CS_2(int64,account)  ),
             CS_ARGV(,CS_3(int64,account)  ),
             CS_BODY( CS_4(int64,account)  )
             )
CS_MAKE_TYPE(ac_info_all_back,_sc_,
             CS_ARGV( CS_1(int64,account),CS_1(int64,phone),CS_1(int64,age),CS_1(int64,sex),CS_1(string,nickname),CS_1(string,location),CS_1(string,icon)  ),
             CS_BODY( CS_2(int64,account) CS_2(int64,phone) CS_2(int64,age) CS_2(int64,sex) CS_2(string,nickname) CS_2(string,location) CS_2(string,icon)  ),
             CS_ARGV(,CS_3(int64,account),CS_3(int64,phone),CS_3(int64,age),CS_3(int64,sex),CS_3(string,nickname),CS_3(string,location),CS_3(string,icon)  ),
             CS_BODY( CS_4(int64,account) CS_4(int64,phone) CS_4(int64,age) CS_4(int64,sex) CS_4(string,nickname) CS_4(string,location) CS_4(string,icon)  )
             )

//!
//! ac_update_info : 更新账号信息
//!
//! extend :
//!     uint account        账号
//!     uint phone          电话
//!     uint age            年龄
//!     uint sex            性别
//!     string nickname     昵称
//!     string location     地址
//!     string icon         头像
//!
//! back :
//!     uint account        账号
//!     string icon         头像
//!     bool ok             确认
//!
//! （ uint 类型等于 -1 时，不修改，string 类型为空时，不修改 ）
//!
CS_MAKE_TYPE(ac_update_info,_cs_,
             CS_ARGV( CS_1(int64,account),CS_1(int64,phone),CS_1(int64,age),CS_1(int64,sex),CS_1(string,nickname),CS_1(string,location),CS_1(string,icon)  ),
             CS_BODY( CS_2(int64,account) CS_2(int64,phone) CS_2(int64,age) CS_2(int64,sex) CS_2(string,nickname) CS_2(string,location) CS_2(string,icon)  ),
             CS_ARGV(,CS_3(int64,account),CS_3(int64,phone),CS_3(int64,age),CS_3(int64,sex),CS_3(string,nickname),CS_3(string,location),CS_3(string,icon)  ),
             CS_BODY( CS_4(int64,account) CS_4(int64,phone) CS_4(int64,age) CS_4(int64,sex) CS_4(string,nickname) CS_4(string,location) CS_4(string,icon)  )
             )
CS_MAKE_TYPE(ac_update_info_back,_sc_,
             CS_ARGV( CS_1(int64,account),CS_1(string,icon),CS_1(bool,ok)  ),
             CS_BODY( CS_2(int64,account) CS_2(string,icon) CS_2(bool,ok)  ),
             CS_ARGV(,CS_3(int64,account),CS_3(string,icon),CS_3(bool,ok)  ),
             CS_BODY( CS_4(int64,account) CS_4(string,icon) CS_4(bool,ok)  )
             )

//!
//! ac_update_remarks : 更新好友备注
//!
//! extend :
//!     uint account        账号
//!     uint friends        好友账号
//!     string remarks      备注
//!
//! back :
//!     uint account        账号
//!     uint friends        好友账号
//!     string remarks      备注
//!
CS_MAKE_TYPE(ac_update_remarks,_cs_,
             CS_ARGV( CS_1(int64,account),CS_1(int64,friends),CS_1(string,remarks)  ),
             CS_BODY( CS_2(int64,account) CS_2(int64,friends) CS_2(string,remarks)  ),
             CS_ARGV(,CS_3(int64,account),CS_3(int64,friends),CS_3(string,remarks)  ),
             CS_BODY( CS_4(int64,account) CS_4(int64,friends) CS_4(string,remarks)  )
             )
CS_MAKE_TYPE(ac_update_remarks_back,_sc_,
             CS_ARGV( CS_1(int64,account),CS_1(int64,friends),CS_1(string,remarks)  ),
             CS_BODY( CS_2(int64,account) CS_2(int64,friends) CS_2(string,remarks)  ),
             CS_ARGV(,CS_3(int64,account),CS_3(int64,friends),CS_3(string,remarks)  ),
             CS_BODY( CS_4(int64,account) CS_4(int64,friends) CS_4(string,remarks)  )
             )


////!
////! friends_list : 好友列表
////!
////! extend :
////!     uint account        账号
////!
////! back :
////!     string svec_fs      存储好友列表的数组
////!     bool ok             确认
////!
////!     | svec_fs 存储json格式
////!     | uint account     好友账号
////!
////!     ( svec是存储vector的json数据格式，需要通过set_json_vec 和 get_json_vec 函数获取 )
////!
//CS_MAKE_TYPE(friends_list,_cs_,
//             CS_ARGV( CS_1(int64,account)  ),
//             CS_BODY( CS_2(int64,account)  ),
//             CS_ARGV(,CS_3(int64,account)  ),
//             CS_BODY( CS_4(int64,account)  )
//             )
//CS_MAKE_TYPE(friends_list_back,_sc_,
//             CS_ARGV( CS_1(int64,account),CS_1(string,svec_fs),CS_1(bool,ok)  ),
//             CS_BODY( CS_2(int64,account) CS_2(string,svec_fs) CS_2(bool,ok)  ),
//             CS_ARGV(,CS_3(int64,account),CS_3(string,svec_fs),CS_3(bool,ok)  ),
//             CS_BODY( CS_4(int64,account) CS_4(string,svec_fs) CS_4(bool,ok)  )
//             )

////!
////! friends_status : 好友状态
////!
////! extend :
////!     string svec_ac_fs   好友列表的数组
////!
////! back :
////!     string svec_ac_info 好友列表的数组
////!     bool ok             确认
////!
////!     | svec_ac_info存储json格式： 使用 set_ac_info_json 和 get_ac_info_json 函数解析 )
////!     | uint ac_friends     好友账号
////!     | string nickname     昵称
////!     | string icon         头像
////!     | string remarks      备注
////!     | bool online         在线
////!
////!     ( svec是存储vector的json数据格式，需要通过set_json_vec 和 get_json_vec 函数获取 )
////!
//CS_MAKE_TYPE(friends_status,_cs_,
//             CS_ARGV( CS_1(int64,account),CS_1(string,svec_ac_fs)  ),
//             CS_BODY( CS_2(int64,account) CS_2(string,svec_ac_fs)  ),
//             CS_ARGV(,CS_3(int64,account),CS_3(string,svec_ac_fs)  ),
//             CS_BODY( CS_4(int64,account) CS_4(string,svec_ac_fs)  )
//             )
//CS_MAKE_TYPE(friends_status_back,_sc_,
//             CS_ARGV( CS_1(int64,account),CS_1(string,svec_ac_info),CS_1(bool,ok)  ),
//             CS_BODY( CS_2(int64,account) CS_2(string,svec_ac_info) CS_2(bool,ok)  ),
//             CS_ARGV(,CS_3(int64,account),CS_3(string,svec_ac_info),CS_3(bool,ok)  ),
//             CS_BODY( CS_4(int64,account) CS_4(string,svec_ac_info) CS_4(bool,ok)  )
//             )

//!
//! swap_cache : 转发缓存
//!
//! extend :
//!     uint target         目标账号 [请求方账号]
//!
//! back :
//!     string svec_sjson   暂存信息数组
//!
CS_MAKE_TYPE(swap_cache,_cs_,
             CS_ARGV( CS_1(int64,target)  ),
             CS_BODY( CS_2(int64,target)  ),
             CS_ARGV(,CS_3(int64,target)  ),
             CS_BODY( CS_4(int64,target)  )
             )
CS_MAKE_TYPE(swap_cache_back,_sc_,
             CS_ARGV( CS_1(string,svec_sjson)  ),
             CS_BODY( CS_2(string,svec_sjson)  ),
             CS_ARGV(,CS_3(string,svec_sjson)  ),
             CS_BODY( CS_4(string,svec_sjson)  )
             )


//!
//! swap_msg : 发送消息
//!
//! extend :
//!     uint target         目标账号
//!     uint source         源址账号
//!     uint time_to        发送时间
//!     string type         消息类型 [Text,Img,File]
//!     string content      消息内容
//!
//! back :
//!     uint target         目标账号
//!     uint source         源址账号
//!     uint time_to        发送时间
//!     uint time_ok        确认时间
//!
CS_MAKE_TYPE(swap_msg,_cc_,
             CS_ARGV( CS_1(int64,target),CS_1(int64,source),CS_1(int64,time_to),CS_1(string,type),CS_1(string,content)  ),
             CS_BODY( CS_2(int64,target) CS_2(int64,source) CS_2(int64,time_to) CS_2(string,type) CS_2(string,content)  ),
             CS_ARGV(,CS_3(int64,target),CS_3(int64,source),CS_3(int64,time_to),CS_3(string,type),CS_3(string,content)  ),
             CS_BODY( CS_4(int64,target) CS_4(int64,source) CS_4(int64,time_to) CS_4(string,type) CS_4(string,content)  )
             )
CS_MAKE_TYPE(swap_msg_back,_cc_,
             CS_ARGV( CS_1(int64,target),CS_1(int64,source),CS_1(int64,time_to),CS_1(int64,time_ok)  ),
             CS_BODY( CS_2(int64,target) CS_2(int64,source) CS_2(int64,time_to) CS_2(int64,time_ok)  ),
             CS_ARGV(,CS_3(int64,target),CS_3(int64,source),CS_3(int64,time_to),CS_3(int64,time_ok)  ),
             CS_BODY( CS_4(int64,target) CS_4(int64,source) CS_4(int64,time_to) CS_4(int64,time_ok)  )
             )




//===== test =====
//CS_MAKE_TYPE(test1,_cc_,
//               CS_ARGV(),
//               CS_BODY(),
//               CS_ARGV(),
//               CS_BODY()
//               )
//CS_MAKE_TYPE(test2,_cs_,
//               CS_ARGV(CS_1(uint,account)),
//               CS_BODY(CS_2(uint,account)),
//               CS_ARGV(,CS_3(uint,account)),
//               CS_BODY(CS_4(uint,account))
//               )
//CS_MAKE_TYPE(test3,_sc_,
//               CS_ARGV(CS_1(uint,account),CS_1(string,passwd)),
//               CS_BODY(CS_2(uint,account) CS_2(string,passwd)),
//               CS_ARGV(,CS_3(uint,account),CS_3(string,passwd)),
//               CS_BODY(CS_4(uint,account) CS_4(string,passwd))
//               )
//===== test =====




//===== files =====

#define CS_FILES_JSON 0x01
#define CS_FILES_BINARY 0x02

#define CS_BIT_FLG 1
#define CS_BIT_ID 8

//#define CS_SENDBUF_HIGH_LINE    (1U << 23)  // 8M

template <class T_ct>
static string ct_s(T_ct ct)
{ return string((char*)&ct,sizeof(T_ct)); }

template <class T_ct>
static T_ct st_c(const string &str)
{ return *(T_ct*)str.c_str(); }

static char check_files_flg(const string &msg)
{
    char flg = 0x00;
    if(msg.size() < 0) return flg;
    flg = msg[0];
    return flg;
}

static string set_files_json(const string &sjson)
{
    char flg = CS_FILES_JSON;
    string str = flg + sjson;
    return str;
}
static bool get_files_json(const string &msg,string &str)
{
    if(msg.size() == 0) return false;
    str = string(msg.begin()+1,msg.end());
    return true;
}

static string set_files_binary(int64 id,const string &sjson)
{
    char flg = CS_FILES_BINARY;
    string str_id = ct_s(id);
    string str = flg + str_id + sjson;
    return str;
}
static bool get_files_binary(const string &msg,int64 &id,string &buf)
{
    bool ret = false;
    if(msg.size() < CS_BIT_FLG + CS_BIT_ID) return ret;

    try {
        string str_id(msg.begin() +CS_BIT_FLG,msg.begin() +CS_BIT_FLG +CS_BIT_ID);
        id = st_c<int64>(str_id);

        buf.resize(msg.end() - (msg.begin() +CS_BIT_FLG +CS_BIT_ID));
        std::copy(msg.begin() +CS_BIT_FLG +CS_BIT_ID,msg.end(),buf.begin());
        ret = true;
    } catch(...) {}
    return ret;
}


//!
//! files_create_upload : 上传文件
//!
//! extend :
//!     uint length_file    文件长度
//!     string abs_file     文件路径
//!     string save_file    保存路径
//!
//! back :
//!     uint id             临时命名
//!     string abs_file     文件路径
//!     bool ok             反馈成功
//!
CS_MAKE_TYPE(files_create_upload,_cs_,
             CS_ARGV( CS_1(int64,length_file),CS_1(string,abs_file),CS_1(string,save_file)  ),
             CS_BODY( CS_2(int64,length_file) CS_2(string,abs_file) CS_2(string,save_file)  ),
             CS_ARGV(,CS_3(int64,length_file),CS_3(string,abs_file),CS_3(string,save_file)  ),
             CS_BODY( CS_4(int64,length_file) CS_4(string,abs_file) CS_4(string,save_file)  )
             )
CS_MAKE_TYPE(files_create_upload_back,_sc_,
             CS_ARGV( CS_1(int64,id),CS_1(string,abs_file),CS_1(bool,ok)  ),
             CS_BODY( CS_2(int64,id) CS_2(string,abs_file) CS_2(bool,ok)  ),
             CS_ARGV(,CS_3(int64,id),CS_3(string,abs_file),CS_3(bool,ok)  ),
             CS_BODY( CS_4(int64,id) CS_4(string,abs_file) CS_4(bool,ok)  )
             )

//!
//! files_begin_upload : 开始上传 [ 客户端请求 ] [ 分段请求 ]
//!
//! extend :
//!     uint id         临时命名
//!
//! back :
//!     uint id         临时命名
//!
CS_MAKE_TYPE(files_begin_upload,_cs_,
             CS_ARGV( CS_1(int64,id)  ),
             CS_BODY( CS_2(int64,id)  ),
             CS_ARGV(,CS_3(int64,id)  ),
             CS_BODY( CS_4(int64,id)  )
             )
CS_MAKE_TYPE(files_begin_upload_back,_sc_,
             CS_ARGV( CS_1(int64,id)  ),
             CS_BODY( CS_2(int64,id)  ),
             CS_ARGV(,CS_3(int64,id)  ),
             CS_BODY( CS_4(int64,id)  )
             )

//!
//! files_finish_upload : 上传完成
//!
//! extend :
//!     uint id             临时命名
//!     bool finish         上传反馈
//!
//! back :
//!     uint id             临时命名
//!     bool ok             反馈成功
//!
CS_MAKE_TYPE(files_finish_upload,_cs_,
             CS_ARGV( CS_1(int64,id),CS_1(bool,finish)  ),
             CS_BODY( CS_2(int64,id) CS_2(bool,finish)  ),
             CS_ARGV(,CS_3(int64,id),CS_3(bool,finish)  ),
             CS_BODY( CS_4(int64,id) CS_4(bool,finish)  )
             )
CS_MAKE_TYPE(files_finish_upload_back,_sc_,
             CS_ARGV( CS_1(int64,id),CS_1(bool,ok)  ),
             CS_BODY( CS_2(int64,id) CS_2(bool,ok)  ),
             CS_ARGV(,CS_3(int64,id),CS_3(bool,ok)  ),
             CS_BODY( CS_4(int64,id) CS_4(bool,ok)  )
             )

//!
//! files_create_download : 下载文件
//!
//! extend :
//!     string abs_path     下载文件
//!     string save_path    保存路径
//!
//! back :
//!     uint id             临时命名
//!     uint length_file    文件长度
//!     string abs_path     下载文件
//!     string save_path    文件名称
//!     bool ok             反馈成功
//!
CS_MAKE_TYPE(files_create_download,_cs_,
             CS_ARGV( CS_1(string,abs_path),CS_1(string,save_path)  ),
             CS_BODY( CS_2(string,abs_path) CS_2(string,save_path)  ),
             CS_ARGV(,CS_3(string,abs_path),CS_3(string,save_path)  ),
             CS_BODY( CS_4(string,abs_path) CS_4(string,save_path)  )
             )
CS_MAKE_TYPE(files_create_download_back,_sc_,
             CS_ARGV( CS_1(int64,id),CS_1(int64,length_file),CS_1(string,abs_path),CS_1(string,save_path),CS_1(bool,ok)  ),
             CS_BODY( CS_2(int64,id) CS_2(int64,length_file) CS_2(string,abs_path) CS_2(string,save_path) CS_2(bool,ok)  ),
             CS_ARGV(,CS_3(int64,id),CS_3(int64,length_file),CS_3(string,abs_path),CS_3(string,save_path),CS_3(bool,ok)  ),
             CS_BODY( CS_4(int64,id) CS_4(int64,length_file) CS_4(string,abs_path) CS_4(string,save_path) CS_4(bool,ok)  )
             )

//!
//! files_begin_download : 开始下载 [ 客户端请求 ] [ 分段请求 ]
//!
//! extend :
//!     uint id         临时命名
//!
//! back :
//!     uint id         临时命名
//!
CS_MAKE_TYPE(files_begin_download,_cs_,
             CS_ARGV( CS_1(int64,id)  ),
             CS_BODY( CS_2(int64,id)  ),
             CS_ARGV(,CS_3(int64,id)  ),
             CS_BODY( CS_4(int64,id)  )
             )
CS_MAKE_TYPE(files_begin_download_back,_sc_,
             CS_ARGV( CS_1(int64,id)  ),
             CS_BODY( CS_2(int64,id)  ),
             CS_ARGV(,CS_3(int64,id)  ),
             CS_BODY( CS_4(int64,id)  )
             )

//!
//! files_finish_download : 下载结束 [ 服务器下发 ]
//!
//! extend :
//!     uint id         临时命名
//!     bool ok         反馈成功
//!
//! back :
//!     uint id         临时命名
//!     bool ok         反馈成功
//!
CS_MAKE_TYPE(files_finish_download,_cs_,
             CS_ARGV( CS_1(int64,id),CS_1(bool,ok)  ),
             CS_BODY( CS_2(int64,id) CS_2(bool,ok)  ),
             CS_ARGV(,CS_3(int64,id),CS_3(bool,ok)  ),
             CS_BODY( CS_4(int64,id) CS_4(bool,ok)  )
             )
CS_MAKE_TYPE(files_finish_download_back,_sc_,
             CS_ARGV( CS_1(int64,id),CS_1(bool,ok)  ),
             CS_BODY( CS_2(int64,id) CS_2(bool,ok)  ),
             CS_ARGV(,CS_3(int64,id),CS_3(bool,ok)  ),
             CS_BODY( CS_4(int64,id) CS_4(bool,ok)  )
             )

//===== files =====




} // protocol

#endif // WEB_PROTOCOL_H
