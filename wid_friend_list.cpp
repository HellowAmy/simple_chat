#include "wid_friend_list.h"

#include "Tvlog.h"

wid_friend_list::wid_friend_list(QWidget *parent)
    : qframe_line(parent)
{
    QSize max_size = QSize(250 + 500,500);
    QSize max_show = QSize(500,500);
    QSize max_area = QSize(240,350);
    QSize max_info = QSize(240,140);

    _space = 5;
    _size_icon = QSize(240 - _space*3,40);
    _show_friend = nullptr;
    this->resize(max_size);

    _wid_chat = new QWidget(this);
    _wid_chat->resize(max_show);
    _wid_chat->show();

    _wid_info = new wid_friend_info(this);
    _wid_info->resize(max_info);
    _wid_info->set_icon("../pic/one.png");
    _wid_info->set_name("帕斯的");
    _wid_info->set_extend({
        "搜索好友",
        "添加好友",
        "分组管理",
        "切换账号",
        "设置",
        "退出"
    });
    _wid_info->update_info();

    qframe_line *wid = new qframe_line(this);
    _wid_area = new qarea_wid(wid);
    _wid_area->init_size(wid->calc_size(max_area,_space));
    wid->resize_center(_wid_area,max_area,_space);

    {
        qmove_pos move;
        move.add_wid(_wid_info);
        move.add_wid(wid);
        move.move_y(QPoint(0,0),_space*2);
    }
    {
        qmove_pos move;
        move.add_wid(_wid_info);
        move.add_wid(_wid_chat);
        move.move_x(QPoint(0,0),_space*2);
    }

    _wid_setting = new QWidget(this);
    _wid_setting->move(_wid_chat->pos());
    _wid_setting->resize(_wid_chat->size());
    _wid_setting->hide();
    _wid_info->get_extend_wid()->setParent(_wid_setting);

    connect(_wid_info,&wid_friend_info::sn_click_extend,this,[=](bool extend,QString tips){
        auto fn_show_wid = [=](){
            if(_wid_info->get_extend_wid()->isHidden() == true)
            {
                _wid_setting->hide();
                _wid_chat->show();
            }
            else
            {
                _wid_setting->show();
                _wid_chat->hide();
            }
        };

        if(extend)
        {
            _wid_setting->show();
            _wid_chat->hide();
            fn_show_wid();
        }
        else
        {
            qlog("<<"<<tips);
//            qDebug<<"<<"<<tips;
        }

    });
}

void wid_friend_list::set_icon(QString icon)
{
    _icon = icon;
}

void wid_friend_list::add_friend(const ct_info &info)
{
    //按钮生成
    wid_friend_butt *butt = new wid_friend_butt(_wid_area);
    butt->set_info(info.icon,info.name);
    butt->init_size(_size_icon);
    _wid_area->add_wid(butt,_space);

    //根据是否为在线状态，标记颜色
    ch_connect_col(butt,info.connect);

    //组合按钮与聊天框
    std::shared_ptr<ct_friend> sp_friend = std::make_shared<ct_friend>();
    sp_friend->info = info;
    sp_friend->butt = butt;
    sp_friend->chat = nullptr;
    _map_friends.insert(info.id,sp_friend);

    //按钮信号槽连接聊天框
    connect(sp_friend->butt->get_butt(),&qbutt_line::sn_clicked,this,[=](){

        make_chat(sp_friend.get()); //首次进入时生成聊天窗口

        ch_keep_col(sp_friend->butt,sp_friend->info.connect); //点击时保持按钮选中

        update_show_friend(sp_friend.get()); //更新聊天窗口句柄，取消上一个窗口选中状态颜色
    });
}

void wid_friend_list::update_connect(long long id, bool connect)
{
    auto it =_map_friends.find(id);
    if(it != _map_friends.end())
    {
        ct_friend *ct = it.value().get();
        ct->info.connect = connect;
        ch_connect_col(ct->butt,connect);
//        ch_keep_col(ct->butt,connect);
    }







}

void wid_friend_list::ch_connect_col(wid_friend_butt *butt, bool connect)
{
    if(connect) butt->get_frame()->set_col(Qt::darkGreen);
    else butt->get_frame()->set_col(Qt::darkRed);
}

void wid_friend_list::ch_keep_col(wid_friend_butt *butt, bool connect)
{
    butt->get_butt()->keep_status(true);
    if(connect) butt->get_butt()->set_col(Qt::darkGreen);
    else butt->get_butt()->set_col(Qt::darkRed);
}

void wid_friend_list::update_show_friend(ct_friend *ct)
{
    if(_show_friend != nullptr)
    {
        if(_show_friend->chat != ct->chat)
        {
            _show_friend->butt->get_butt()->keep_status(false);
            _show_friend->butt->get_butt()->set_col(Qt::darkGray);
            _show_friend->chat->close();
        }
    }
    _show_friend = ct;
    _show_friend->chat->show();
}

#include <QTimer>
void wid_friend_list::make_chat(ct_friend *ct)
{
    if(ct->chat == nullptr)
    {
        wid_chat *chat = new wid_chat(_wid_chat);
        chat->get_input()->set_icon(ct->info.icon,_icon);
        ct->chat = chat;

        //发送到到网络转发端,附带账号与时间用于定位回馈确认消息
        connect(ct->chat->get_input(),&wid_chat_input::sn_send_msg,this,[=](wid_message *msg){
            ct_msg cts;
            cts.id = ct->info.id;
            cts.time = msg->get_info().time;
            cts.msg = msg;
            emit sn_send_msg(cts);
        });

        //===== 自动问候
        auto msg = chat->get_input()->make_text("你好，我是"+ct->info.name,Qt::AlignLeft);
        chat->get_output()->add_message(msg);
        QTimer *ti = new QTimer(this);
        ti->start(5000);
        connect(ti,&QTimer::timeout,[=](){
            if(ct == _show_friend)
            {
                auto msg = chat->get_input()->make_msg
                           (new wid_msg_hint(QDateTime::currentDateTime().toString()),Qt::AlignCenter);
                chat->get_output()->add_message(msg);
                auto msg2 = chat->get_input()->make_msg
                           (new wid_msg_hint("对方撤回消息"),Qt::AlignCenter);
                chat->get_output()->add_message(msg2);
            }

        });
        //===== 自动问候
    }
}

