#ifndef WEB_PROTOCOL_H
#define WEB_PROTOCOL_H

#include "include/nlohmann/json.hpp"

#include <string>
#include <vector>

typedef long long int64;
typedef nlohmann::json json;
using std::string;
using std::vector;

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



#define CS_1(in_type,in_value) in_type in_value
#define CS_2(in_type,in_value) set_extend<in_type>(js,#in_value,in_value);
#define CS_3(in_type,in_value) in_type &in_value
#define CS_4(in_type,in_value) in_value = get_extend<in_type>(js,#in_value);

#define CS_ARGV(...) __VA_ARGS__
#define CS_BODY(...) __VA_ARGS__
#define CS_MAKE_TYPE(in_type,in_stream,in_sarg,in_sbody,in_garg,in_gbody)     \
    static string in_type = #in_type;                                           \
    static string set_##in_type(in_sarg)                                        \
    {                                                                           \
        string sjson = make_json(in_stream,in_type);                            \
        json js = json::parse(sjson);                                           \
        in_sbody                                                                \
        return js.dump();                                                       \
    }                                                                           \
    static bool get_##in_type(const string &sjson in_garg)                      \
    {                                                                           \
        json js = json::parse(sjson);                                           \
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
//! login : 登陆账号
//!
//! extend :
//!     uint account    账号
//!     string passwd   密码
//!
//! back :
//!     bool ok         确认
//!     string info     提示信息
//!
CS_MAKE_TYPE(login,_cs_,
        CS_ARGV( CS_1(int64,account),CS_1(string,passwd)  ),
        CS_BODY( CS_2(int64,account) CS_2(string,passwd)  ),
        CS_ARGV(,CS_3(int64,account),CS_3(string,passwd)  ),
        CS_BODY( CS_4(int64,account) CS_4(string,passwd)  )
        )
CS_MAKE_TYPE(login_back,_sc_,
        CS_ARGV( CS_1(bool,ok)  ),
        CS_BODY( CS_2(bool,ok)  ),
        CS_ARGV(,CS_3(bool,ok)  ),
        CS_BODY( CS_4(bool,ok)  )
        )

//!
//! friends_list : 好友列表
//!
//! extend :
//!     uint account        账号
//!
//! back :
//!     string svec_fs      存储好友列表的数组
//!     bool ok             确认
//!
//!     ( svec是存储vector的json数据格式，需要通过get_json_vec函数获取 )
//!
CS_MAKE_TYPE(friends_list,_cs_,
             CS_ARGV( CS_1(int64,account)  ),
             CS_BODY( CS_2(int64,account)  ),
             CS_ARGV(,CS_3(int64,account)  ),
             CS_BODY( CS_4(int64,account)  )
)
CS_MAKE_TYPE(friends_list_back,_sc_,
             CS_ARGV( CS_1(string,svec_fs),CS_1(bool,ok)  ),
             CS_BODY( CS_2(string,svec_fs) CS_2(bool,ok)  ),
             CS_ARGV(,CS_3(string,svec_fs),CS_3(bool,ok)  ),
             CS_BODY( CS_4(string,svec_fs) CS_4(bool,ok)  )
)

//!
//! friends_status : 好友状态
//!
//! extend :
//!     uint ac_friends     好友账号
//!
//! back :
//!     uint ac_friends     好友账号
//!     string nickname     昵称
//!     string icon         头像
//!     bool online         在线
//!     bool ok             确认
//!
CS_MAKE_TYPE(friends_status,_cs_,
             CS_ARGV( CS_1(int64,ac_friends)  ),
             CS_BODY( CS_2(int64,ac_friends)  ),
             CS_ARGV(,CS_3(int64,ac_friends)  ),
             CS_BODY( CS_4(int64,ac_friends)  )
)
CS_MAKE_TYPE(friends_status_back,_sc_,
             CS_ARGV( CS_1(int64,ac_friends),CS_1(string,nickname),CS_1(string,icon),CS_1(bool,online),CS_1(bool,ok)  ),
             CS_BODY( CS_2(int64,ac_friends) CS_2(string,nickname) CS_2(string,icon) CS_2(bool,online) CS_2(bool,ok)  ),
             CS_ARGV(,CS_3(int64,ac_friends),CS_3(string,nickname),CS_3(string,icon),CS_3(bool,online),CS_3(bool,ok)  ),
             CS_BODY( CS_4(int64,ac_friends) CS_4(string,nickname) CS_4(string,icon) CS_4(bool,online) CS_4(bool,ok)  )
)

//!
//! swap_msg : 发送消息
//!
//! extend :
//!     uint target         目标账号
//!     uint source         源址账号
//!     long long time_to   发送时间
//!     string type         消息类型 [Text,Img,File]
//!     string content      消息内容
//!
//! back :
//!     uint target         目标账号
//!     uint source         源址账号
//!     long long time_to   发送时间
//!     long long time_ok   确认时间
//!     bool ok             反馈成功
//!
CS_MAKE_TYPE(swap_msg,_cc_,
        CS_ARGV( CS_1(int64,target),CS_1(int64,source),CS_1(int64,time_to),CS_1(string,type),CS_1(string,content)  ),
        CS_BODY( CS_2(int64,target) CS_2(int64,source) CS_2(int64,time_to) CS_2(string,type) CS_2(string,content)  ),
        CS_ARGV(,CS_3(int64,target),CS_3(int64,source),CS_3(int64,time_to),CS_3(string,type),CS_3(string,content)  ),
        CS_BODY( CS_4(int64,target) CS_4(int64,source) CS_4(int64,time_to) CS_4(string,type) CS_4(string,content)  )
        )
CS_MAKE_TYPE(swap_msg_back,_cc_,
        CS_ARGV( CS_1(int64,target),CS_1(int64,source),CS_1(int64,time_to),CS_1(int64,time_ok),CS_1(bool,ok)  ),
        CS_BODY( CS_2(int64,target) CS_2(int64,source) CS_2(int64,time_to) CS_2(int64,time_ok) CS_2(bool,ok)  ),
        CS_ARGV(,CS_3(int64,target),CS_3(int64,source),CS_3(int64,time_to),CS_3(int64,time_ok),CS_3(bool,ok)  ),
        CS_BODY( CS_4(int64,target) CS_4(int64,source) CS_4(int64,time_to) CS_4(int64,time_ok) CS_4(bool,ok)  )
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

} // protocol

#endif // WEB_PROTOCOL_H
