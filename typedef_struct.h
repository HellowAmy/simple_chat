#ifndef TYPEDEF_STRUCT_H
#define TYPEDEF_STRUCT_H

#include <QString>
#include <QDateTime>
#include <string>

#include <QLineEdit>
#include <QLabel>
//#include <string>
#include <tuple>

#include "qlab_img.h"
#include "qframe_line.h"

#define stoqs(s) QString::fromStdString(s)
#define qstos(s) s.toStdString()

typedef long long int64;
using std::string;

namespace typedef_struct {

//!
//! 显示区消息类型
//!
static string _types_Img_     = "Img";
static string _types_Text_    = "Text";
static string _types_File_    = "File";
static string _types_Hint_    = "Hint";


//!
//! 发送者身份对应的对齐方式
//!
static string _object_AR_     = "AR";
static string _object_AL_     = "AL";
static string _object_SYS_    = "SYS";


//== 带边框界面 ==
static auto fn_time_now = []()
    -> int64
{
    return QDateTime::currentMSecsSinceEpoch();
};

static auto fn_LineEdit = [](QWidget *wid,QSize size = QSize(100,30),QPoint pos = QPoint(0,0))
    -> std::tuple<qframe_line*,QLineEdit*>
{
    QString sty = R"(
            QLineEdit {
                font-family: 微软雅黑;
                border: none;
                font-size: 14px;
                color: #808080;
            }
            )";
    qframe_line *fr = new qframe_line(wid);
    QLineEdit *ed = new QLineEdit(fr);
    ed->setAlignment(Qt::AlignVCenter);
    ed->setStyleSheet(sty);
    ed->show();
    fr->resize_center(ed,size,5);
    fr->move(pos);
    fr->show();
    return std::make_tuple(fr,ed);
};

static auto fn_LineEdit_txt = [](QWidget *wid,QString txt,QSize size)
    -> std::tuple<qframe_line*,QLineEdit*>
{
    auto [c,d] = fn_LineEdit(wid,size);
    QLineEdit *p = d;
    p->setText(txt);
    return std::make_tuple(c,d);
};

static auto fn_LineEdit_non = [](QWidget *wid,QString txt,QSize size)
    -> std::tuple<qframe_line*,QLineEdit*>
{
    auto [c,d] = fn_LineEdit(wid,size);

    qframe_line *f = c;
    f->set_col(QColor(0xaa0000));

    QLineEdit *p = d;
    p->setText(txt);
    p->setAlignment(Qt::AlignCenter);
    p->setFocusPolicy(Qt::NoFocus);
    return std::make_tuple(c,d);
};

static auto fn_Label = [](QWidget *wid,QString str, QSize size = QSize(80,30),QPoint pos = QPoint(0,0))
    -> std::tuple<qframe_line*,QLabel*>
{
    qframe_line *fr = new qframe_line(wid);
    QLabel *lab = new QLabel(fr);
    lab->setText(str);
    lab->setAlignment(Qt::AlignCenter);
    lab->setFont(QFont("微软雅黑",10));
    lab->show();
    fr->resize_center(lab,size,5);
    fr->move(pos);
    fr->show();
    return std::make_tuple(fr,lab);
};

static auto fn_fimg = [](QWidget *wid,QString icon, QSize size = QSize(80,30),QPoint pos = QPoint(0,0))
    -> std::tuple<qframe_line*,qlab_img*>
{
    qframe_line *fr = new qframe_line(wid);
    qlab_img *lab = new qlab_img(fr);
    lab->set_img(icon);
    lab->set_keep(false,QSize(fr->calc_size(size,5)));
    lab->update_img();
    lab->show();
    fr->resize_center(lab,size,5);
    fr->move(pos);
    fr->show();
    return std::make_tuple(fr,lab);
};
//== 带边框界面 ==



//!
//! 输入输出大小关系
//!
static struct
{
    int space_chat = 0;             //input 与 output 的间隔
    int space_msg = 10;             //每条消息的间隔
    int max_width = 500;            //聊天窗口最大宽度
    int height_output = 330+10;     //显示区高度
    int height_input = 160;         //输入区高度
    int input_padding = 5;          //输入区间隔
    int input_extend_butt = 50;     //输入区扩展按钮大小
} _fsize_ioput_;

//!
//! 显示各类型消息大小关系
//!
static struct
{
    int msg_space = 5;                      //消息于头像间隔
    int msg_padding = 5;                    //头像间距
    int msg_icon = 50;                      //消息头像的大小
    int msg_max_width = 500 - msg_space*3;  //消息内容被限制的最大宽度
    int msg_width = msg_max_width - msg_icon*2 - msg_space*2;   //最终消息内容显示长度
} _fsize_message_;


//!
//! 账号信息传递
//!
struct ct_ac_info
{
    int64 account;      //账号
    int64 phone;        //电话
    int64 age;          //年龄
    int64 sex;          //性别
    string nickname;    //昵称
    string location;    //地址
    string icon;        //头像
};

//!
//! 好友状态初始化
//!
struct ct_friends_init
{
    bool online;        //在线
    int64 friends;      //账号
    string nickname;    //昵称
    string icon;        //头像
    string remarks;     //备注
};

//!
//! 输入区发送的消息类型传递
//!
struct ct_msg_type
{
    int64 time;     //时间
    string types;   //类型 [Text,Img,File,Hint]
    string object;  //对象 [AR,AL,SYS]
    string content; //内容
};

//!
//! 发送与接收网络消息的内容类型
//!
struct ct_swap_msg
{
    int64 target;   //目标账号
    int64 source;   //源址账号
    int64 time_to;  //发送时间
    string types;   //消息类型 [Text,Img,File]
    string content; //消息内容
};

//!
//! 网络消息确认反馈类型
//!
struct ct_swap_msg_back
{
    int64 target;   //目标账号
    int64 source;   //源址账号
    int64 time_to;  //发送时间
    int64 time_ok;  //确认时间
};


} // typedef_struct


#endif // TYPEDEF_STRUCT_H
