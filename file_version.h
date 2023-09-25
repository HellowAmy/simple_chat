#ifndef FILE_VERSION_H
#define FILE_VERSION_H

#include <string>

using std::string;

#define CS_HEAD "SMCH_HEAD"
#define CS_VERSION 1

namespace cs_sjson {

//!
//! 交换数据 ： __sjson_swap
//!
//! 头部验证
//! 版本判断
//! 目标账号
//! 发送账号
//! 转发类型 [Swap,Request]
//! 任务类型 [Text,Img,File]
//! 消息内容
//! 扩展内容
static string __sjson_swap = R"(
{
    "head"      : "",
    "version"   : 0,
    "target"    : 0,
    "source"    : 0,
    "type"      : "Swap",
    "task"      : "",
    "content"   : "",
    "extend"    : {}
}
)";


static string __sjson_login = R"(
{
    "head"      : "",
    "version"   : 0,
    "account"   : 0,
    "type"      : "Request",
    "task"      : "Login",
    "passwd"    : "",
    "extend"    : {}
}
)";


} // namespace cs_sjson

#endif // FILE_VERSION_H
