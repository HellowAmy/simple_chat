#include "wid_test.h"
#include "Tvlog.h"
//#include "qbutt_line.h"
//#include "qlab_line.h"
//#include "qframe_line.h"
//#include "qlab_wrap.h"
//#include "qlab_img.h"
//#include "wid_chat_display.h"
//#include "qlab_bubble.h"
//#include "wid_message.h"
//#include "wid_msg_abs.h"
//#include "wid_chat_output.h"
//#include "wid_chat_input.h"
//#include "wid_chat.h"
//#include "wid_chat_list.h"
//#include "wid_friend_butt.h"
#include "wid_friend_list.h"
#include "wid_friend_info.h"
//#include "qsqlist.h"
//#include "include/nlohmann/json.hpp"
//#include "make_json.h"
#include "qweb_client.h"
//#include "web_protocol.h"
//#include "swap_files.h"
//#include "qweb_files.h"
#include "sqlite_op.h"
#include "sqlite_read_history.h"

//#include <filesystem>

//#include <unistd.h>

//#include <QPushButton>
//#include <QTimer>
//#include <QDateTime>
//#include <QLabel>
//#include <QFontMetrics>
//#include <QString>
//#include <QDebug>


void wid_test::test_20(QWidget *parent)
{
    //==
    qweb_client *wc = new qweb_client(this);

    sqlite_read_history *phistory = new sqlite_read_history(this);
    phistory->init_db(wc->get_db());

    wid_friend_list *wid = new wid_friend_list(this);
    wid->hide();

    {
        //登陆：史强 798315362  796304805 607037441 535181553
        int64 account = 607037441;
        string passwd = "123456";
        wid->set_login_info(account,passwd);
    }



    //==
    //连接开启
    connect(wc,&qweb_client::sn_open,this,[=](){
        wc->ask_login(wid->get_account(),wid->get_passwd());
    });
    connect(wc,&qweb_client::sn_close,this,[=](){
        vlogw("sn_close");
    });

    //账号信息
    connect(wc,&qweb_client::sn_ac_login,this,[=]
            (int64 account,string nickname,string icon,vector<ct_friends_init> vec_friends){
        //初始化登陆
        wid->init_login(account,stoqs(nickname),stoqs(icon));
        wid->show();

        //加入好友信息
        for(const auto &a:vec_friends)
        {
            wid_friend_list::ct_friend ct {a,nullptr,nullptr};
            wid->add_friend(ct);
        }
    });

    //消息接收
    connect(wc,&qweb_client::sn_recv_msg,this,[=](ct_swap_msg msg){
        vlogi($(msg.target) $(msg.source) $(msg.time_to) $(msg.types) $(msg.content));

        wid->add_recv_msg(msg);
    });

    //获取 账号/好友 信息
    connect(wc,&qweb_client::sn_ac_info_all,this,[=] (int64 types,ct_ac_info info){
        if(types == en_info_all::e_account) wid->add_person_account(info);
        else if(types == en_info_all::e_friends) wid->add_person_friends(info);
        else if(types == en_info_all::e_ask_ac) sn_connect_t::get()->sn_extend_add_find_back(info);
        else vlogw("sn_ac_info_all types not find" $(types));
    });

    //更新信息
    connect(wc,&qweb_client::sn_update_info,this,[=] (string icon,bool ok){
        if(ok == false)
        { wid_dialog_box().make_box("修改信息失败","服务器更新信息失败，请检查信息合理性",wid_dialog_box::e_error); }

        if(icon == "") wid->get_info()->show_extend();
        else { wid->update_info_icon(); }

    });
    connect(wc,&qweb_client::sn_update_remarks,this,[=] (int64 friends,string remarks){
        wid->add_friend_remarks(friends,remarks);
    });




    //==
    //发送到网络
    connect(wid,&wid_friend_list::sn_send_msg,this,[=](ct_swap_msg ct){
        bool ok = wc->ask_swap_msg(ct);
        vlogfaile(ok,"sn_send_msg failed");
    });

    //请求下载缺失好友头像
    connect(wid,&wid_friend_list::sn_download_icon,this,[=](int64 account){
        auto fn_icon = [=](bool ok,int64 account,string save_path){
            if(ok) wid->update_ac_icon(account,stoqs(save_path));
        };
        qweb_ac_download *wcf = new qweb_ac_download(this);
        wcf->download_icon_ac(account,fn_icon);
        wcf->open();
    });

    //读取历史记录
    connect(wid,&wid_friend_list::sn_history_read_ac,this,[=](int64 account,bool is_non_read){
        vector<ct_msg_type> vec;
        if(is_non_read) phistory->read_history_non_read(account,vec);
        else phistory->read_history(account,vec);

        wid->set_history_msg(account,vec);
    });

    //更新头像
    connect(wid,&wid_friend_list::sn_upload_icon,this,[=](int64 account, string icon){

        auto fn_icon = [=](bool ok,int64 account,string save_path){

            vlogw($(ok)$(account)$(save_path));

            //复制文件上传到头像路径
            if(ok)
            {
                string icon = "icon_"+ std::to_string(account);
                string path_ac = qstos(wid->get_path_icon()) + icon;
                files_info::copy_file(save_path,path_ac);
                vlogw($(save_path)$(path_ac));

                ct_ac_info ct{account,-1,-1,-1,"","",icon};
                wid->sn_update_info(ct);
            }
        };
        qweb_ac_upload *wcf = new qweb_ac_upload(this);
        wcf->upload_icon_ac(account,icon,fn_icon);
        wcf->open();
    });


    //请求 账号/好友 信息
    connect(wid,&wid_friend_list::sn_account_info,this,[=](int64 types,int64 account){
        bool ok = wc->ask_info_all(types,account);
        vlogfaile(ok,$(ok));
    });

    //更新账号信息
    connect(wid,&wid_friend_list::sn_update_info,this,[=](ct_ac_info ct){
        bool ok = wc->ask_update_info(ct);
        vlogfaile(ok,$(ok));
    });
    connect(wid,&wid_friend_list::sn_update_remarks,this,[=]
            (int64 account, int64 friends, string remarks){
        bool ok = wc->ask_update_remarks(account,friends,remarks);
        vlogfaile(ok,$(ok));
    });


    //==
    connect(sn_connect_t::get(),&sn_connect::sn_extend_add_find,this,[=] (int64 types,int64 account){
        bool ok = wc->ask_info_all(types,account);
        vlogfaile(ok,$(ok));
    });


    //==
    wc->open();
}

