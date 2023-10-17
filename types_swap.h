#ifndef TYPES_SWAP_H
#define TYPES_SWAP_H

#include <string>

typedef long long int64;
using std::string;

namespace types_swap {

//typedef_struct

//
struct ct_msg_type
{
    int64 time;     //时间
    string types;   //类型 [Text,Img,File]
    string object;  //对齐
    string content; //内容
};


//
struct ct_swap_msg
{
    int64 target;   //目标账号
    int64 source;   //源址账号
    int64 time_to;  //发送时间
    string types;   //消息类型 [Text,Img,File]
    string content; //消息内容
};


} // types_swap






#endif // TYPES_SWAP_H
