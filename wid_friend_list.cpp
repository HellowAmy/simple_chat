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
    _path_temp = "../temp_file/";
    _path_icon = "../data/head_icon/";
    this->resize(max_size);

    //好友聊天
    _wid_chat = new QWidget(this);
    _wid_chat->resize(max_show);
    _wid_chat->show();

    //好友信息
    _wid_info = new wid_friend_info(this);
    _wid_info->resize(max_info);


    //好友列表
    qframe_line *wid = new qframe_line(this);
    _wid_area = new qarea_wid(wid);
    _wid_area->init_size(wid->calc_size(max_area,_space));
    wid->resize_center(_wid_area,max_area,_space);

    //布局对齐
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

    //添加设置
    _wid_setting = new QWidget(this);
    _wid_setting->move(_wid_chat->pos());
    _wid_setting->resize(_wid_chat->size());
    _wid_setting->hide();
    _wid_info->set_extend_wid(_wid_setting);

    //好友展示区域
    _wid_person = new QWidget(this);
    _wid_person->move(_wid_chat->pos());
    _wid_person->resize(_wid_chat->size());
    _wid_person->hide();

    //好友信息展示
    _wid_fd_info = new wid_person_info(_wid_person);
    _wid_fd_info->init_friends();

    //管理显示界面
    _vec_extend.push_back(_wid_chat);
    _vec_extend.push_back(_wid_setting);
    _vec_extend.push_back(_wid_person);

    //设置与聊天窗口切换
    connect(_wid_info,&wid_friend_info::sn_show_extend,this,[=](){
        show_wid_extend(_wid_setting);

//        if(show) show_wid_extend(_wid_setting);
////        {


////            _wid_chat->hide();
////            _wid_setting->show();
////        }
//        else show_wid_extend(_wid_chat);
////        {
////            _wid_chat->show();
////            _wid_setting->hide();
////        }
    });

    //接收网络消息
    connect(this,&wid_friend_list::sn_update_msg,this,[=](int64 account){
        emit sn_history_read_ac(account); //读取历史记录--未读
    });

//    //退出个人信息修改
//    connect(_wid_fd_info,&wid_person_info::sn_not_change,[=](){
////        show_wid_extend(_wid_chat);
//    });

    connect(_wid_info,&wid_friend_info::sn_info_all,this,[=](){
        emit sn_account_info(en_info_all::e_account,_account);
    });


//    connect(_wid_info->get_person(),&wid_person_info::sn_save_info,this,[=](ct_ac_info ct){
//        emit sn_update_info(ct);
//    });
//    connect(_wid_fd_info,&wid_person_info::sn_save_remarks,this,[=](ct_ac_info ct){
////        int64 account, int64 friends, string remarks
//        int64 account = wid->get_account();
//        int64 ac_friend = wid->get_person()->get_edit_info().account;
//        bool ok = wc->ask_update_remarks(account,ac_friend,remarks);
//        vlogfaile(ok,$("ask_update_remarks") $(remarks));

//        emit sn_update_remarks({account,friends,remarks});
////        emit sn_update_info(ct);
//    });


    connect(_wid_info,&wid_friend_info::sn_change_icon,this,[=](QString file){
        emit sn_upload_icon(_account, qstos(file));

//        ct_ac_info ct{_account,-1,-1,-1,"","",qstos(file)};
//        emit sn_update_info(ct);
    });

    //修改信息
    connect(_wid_info->get_person(),&wid_person_info::sn_save_info,this,[=](ct_ac_info ct){
        emit sn_update_info(ct);
    });
    connect(_wid_fd_info,&wid_person_info::sn_save_remarks,this,[=](string remarks){
        int64 account = _account;
        int64 friends = _wid_fd_info->get_edit_info().account;
        emit sn_update_remarks(account,friends,remarks);
    });

}

void wid_friend_list::set_login_info(int64 account, string passwd)
{
    _account = account;
    _passwd = passwd;
}

void wid_friend_list::update_info_icon()
{
    QString path = _path_icon + "icon_" + stoqs(std::to_string(_account));
    _wid_info->set_icon(path);
    vlogw(Q$(path));
}

void wid_friend_list::add_friend_remarks(int64 account, string remarks)
{
    auto it =_map_friends.find(account);
    if(it != _map_friends.end())
    {
        it->second->status.remarks = remarks;
        wid_friend_butt *butt = it->second->butt;
        butt->get_status()->remarks = remarks;
        butt->update_butt();

        _wid_fd_info->set_info_remarks(stoqs(remarks));
        _wid_fd_info->update_info();
    }
}

void wid_friend_list::add_person_account(ct_ac_info ct)
{
    _wid_info->get_person()->set_info(ct);
    _wid_info->get_person()->update_info();
}

void wid_friend_list::add_person_friends(ct_ac_info ct)
{
    auto it =_map_friends.find(ct.account);
    if(it != _map_friends.end())
    {
        QString remarks = stoqs(it->second->status.remarks);
        QString icon = stoqs(it->second->status.icon);

        _wid_fd_info->set_info(ct);
        _wid_fd_info->set_info_remarks(remarks,icon);
        _wid_fd_info->update_info();
        show_wid_extend(_wid_person);
    }
}

//void wid_friend_list::add_account_person(ct_ac_info ct)
//{
//    int64 account = ct.account;
//    if(account == _account) //登陆账号信息
//    {
//        _wid_info->get_person()->set_info(ct);
//        _wid_info->get_person()->update_info();
//    }
//    else //好友信息
//    {
//        auto it =_map_friends.find(account);
//        if(it != _map_friends.end())
//        {
//            QString remarks = stoqs(it->second->status.remarks);
//            QString icon = stoqs(it->second->status.icon);

//            _wid_fd_info->set_info(ct);
//            _wid_fd_info->set_info_remarks(remarks,icon);
//            _wid_fd_info->update_info();
//            show_wid_extend(_wid_person);
//        }
//    }
//}


void wid_friend_list::update_ac_icon(int64 account,QString path)
{
    if(account == _account) { _wid_info->set_icon(path); }
    else
    {
        auto it = _map_friends.find(account);
        if(it != _map_friends.end())
        {
            wid_friend_butt *butt = it->second->butt;
            butt->get_status()->icon = qstos(path);
            butt->update_butt();
        }
    }
}

void wid_friend_list::init_login(int64 account,QString nickname, QString icon)
{
    _account = account;
    _icon = _path_icon + icon;
    _wid_info->set_name(nickname);

    if(files_info::is_exists(qstos(_icon))) { _wid_info->set_icon(_icon); }
    else { emit sn_download_icon(_account); }
}

void wid_friend_list::set_history_msg(int64 account, const std::vector<ct_msg_type> &vec)
{
    //显示信息到屏幕
    auto it = get_friend(account);
    if(it != nullptr)
    {
        for(auto a:vec)
        { show_msg_history(it,a); }
    }
    else vlogw("set_history_msg: friend not failed" $(account));
}

wid_person_info *wid_friend_list::get_person()
{
    return _wid_fd_info;
}

wid_friend_info *wid_friend_list::get_info()
{
    return _wid_info;
}

int64 wid_friend_list::get_account()
{
    return _account;
}

string wid_friend_list::get_passwd()
{
    return _passwd;
}

QString wid_friend_list::get_path_icon()
{
    return _path_icon;
}

void wid_friend_list::add_friend(ct_friend ct)
{
    //补全头像路径
    ct.status.icon = qstos(_path_icon) + ct.status.icon;

    //按钮生成
    wid_friend_butt *butt = new wid_friend_butt(_wid_area);
//    butt->get_status()
    butt->set_status(ct.status);

    //设置头像,不存在则下载头像
    if(files_info::is_exists(ct.status.icon) == false)
    { emit sn_download_icon(ct.status.friends); }


//    if(files_info::is_exists(qstos(path))) { butt->set_icon(path); }
//    else { emit sn_download_icon(ct.status.account); }

//    butt->set_remarks(stoqs(ct.status.remarks));
//    butt->set_name(stoqs(ct.status.nickname));

    butt->init_size(_size_icon);
    _wid_area->add_wid(butt,_space);

    //根据是否为在线状态，标记颜色
    ch_connect_col(butt,ct.status.online);

    //组合按钮与聊天框
    std::shared_ptr<ct_friend> sp_friend = std::make_shared<ct_friend>(ct);
    sp_friend->butt = butt;
    sp_friend->chat = nullptr;
    _map_friends.emplace(ct.status.friends,sp_friend);

    //按钮信号槽连接聊天框
    connect(sp_friend->butt->get_butt(),&qbutt_line::sn_clicked,this,[=](){

        //首次进入时生成聊天窗口
        make_chat(sp_friend.get());

        //点击时保持按钮选中
        ch_keep_col(sp_friend->butt,sp_friend->status.online);

        //更新聊天窗口句柄，取消上一个窗口选中状态颜色
        update_show_friend(sp_friend.get());

        //底部回滚
        show_bottom_bar();

        //
        show_wid_extend(_wid_chat);
    });

    //获取好友状态信息
    connect(butt,&wid_friend_butt::sn_account_info,this,[=](int64 account){
        emit sn_account_info(en_info_all::e_friends,account);
        vlogi($(account));
    });
}

void wid_friend_list::add_recv_msg(ct_swap_msg ct)
{
    //更新当前显示信息
    int64 account = ct.source;
    auto it = get_friend(account);
    if(it != nullptr)
    {
        if(it->chat != nullptr && it == _show_friend)
        { emit sn_update_msg(account); }
    }
}

void wid_friend_list::update_connect(int64 account, bool online)
{
    auto it =_map_friends.find(account);
    if(it != _map_friends.end())
    {
        it->second->status.online = online;
        ch_connect_col(it->second->butt,online);
    }
}

void wid_friend_list::ch_connect_col(wid_friend_butt *butt, bool connect)
{
    if(connect) butt->get_frame()->set_col(Qt::darkGreen);
    else butt->get_frame()->set_col(Qt::darkRed);
}

void wid_friend_list::ch_keep_col(wid_friend_butt *butt, bool online)
{
    butt->get_butt()->keep_status(true);
    if(online) butt->get_butt()->set_col(Qt::darkGreen);
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

void wid_friend_list::make_chat(ct_friend *ct)
{
    if(ct->chat == nullptr)
    {
        //生成聊天窗口
        QString path = stoqs(ct->status.icon);
        wid_chat *chat = new wid_chat(_wid_chat);
        chat->get_input()->set_icon(path,_icon);
        ct->chat = chat;

        //读取历史记录--全部
        emit sn_history_read_ac(ct->status.friends,false);

        //发送到到网络转发端,附带账号与时间用于定位回馈确认消息
        connect(ct->chat->get_input(),&wid_chat_input::sn_send_msg,this,[=](ct_msg_type msg){
            //发送到网络
            ct_swap_msg cts;
            cts.target  = ct->status.friends;
            cts.source  = _account;
            cts.time_to = msg.time;
            cts.types   = msg.types;
            cts.content = msg.content;
            emit sn_send_msg(cts);

            //回滚到底部
            show_bottom_bar();
        });
    }
}

wid_friend_list::ct_friend *wid_friend_list::get_friend(int64 account)
{
    auto it = _map_friends.find(account);
    if(it != _map_friends.end()) return it->second.get();
    return nullptr;
}

void wid_friend_list::show_msg_history(ct_friend *it,ct_msg_type ct)
{
    auto msg = it->chat->get_input()->make_msg(ct);
    if(msg) it->chat->get_output()->add_message(msg);
}

void wid_friend_list::show_bottom_bar()
{
    _show_friend->chat->get_output()->get_area()->show_bottom_bar();
}

void wid_friend_list::show_wid_extend(QWidget *wid)
{
    for(auto a:_vec_extend)
    {
        if(wid && wid == a) a->show();
        else a->hide();
    }
}


