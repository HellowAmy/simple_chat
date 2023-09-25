#include <iostream>

#include "inter_server.h"
#include "sqlist_op.h"
#include "../../Tvlog.h"

using namespace std;

//网络连接
void test_1()
{
    vlogi("== server ==");

    inter_server sk;

    function fn_open = [](const sp_channe&, const sp_http& http){
        HttpRequest* p = http.get();
        vlogi("fn_open"<<p->path);
    };
    function fn_message = [&](const sp_channe& channe, const string& msg){
        vlogi("fn_message: " << msg);
        hv::WebSocketChannel* p = channe.get();
        p->send(msg);
        if(msg == "exit") sk.get_sv()->stop();
        else if(msg == "close") p->close();
    };
    function fn_close = [&](const sp_channe&){
        vlogi("fn_close");
    };

    sk.func_bind(fn_open,fn_message,fn_close);
    sk.open();

    vlogi("== end ==");
}

//账号密码
void test_2()
{
    sqlist_account sql;

    if(sql.open_account() == false) vloge("failed");
    if(sql.create_account() == false) vloge("failed");

    {
        bool ok = sql.insert_account(123456789,"123456");
        (ok == true ? vlogd($(ok)) : vloge($(ok)));
    }
    {
        bool ok = sql.insert_account(333456789,"1234567");
        (ok == true ? vlogd($(ok)) : vloge($(ok)));
    }
    {
        bool ok = sql.insert_account(444456789,"1234568");
        (ok == true ? vlogd($(ok)) : vloge($(ok)));
    }
    {
        bool ok = sql.insert_account(444456789,"123456");
        (ok == true ? vlogd($(ok)) : vloge($(ok)));
    }
    {
        bool ok = sql.insert_account(544456789,"12345");
        (ok == true ? vlogd($(ok)) : vloge($(ok)));
    }

    {
        vector<string> data;
        bool ok = sql.select_db(sql.get_table(),data);
        (ok == true ? vlogd($(ok)) : vloge($(ok)));
        for(auto a:data)
        {
            vlogi(a);
        }
    }

    {
        bool ok = sql.update_account(123456789,"aabbcc");
        (ok == true ? vlogd($(ok)) : vloge($(ok)));
    }
    {
        bool ok = sql.delete_account(333456789);
        (ok == true ? vlogd($(ok)) : vloge($(ok)));
    }

    {
        vector<string> data;
        bool ok = sql.select_db(sql.get_table(),data);
        (ok == true ? vlogd($(ok)) : vloge($(ok)));
        for(auto a:data)
        {
            vlogi(a);
        }
    }
    {
        bool ok = sql.drop_db(sql.get_table());
        (ok == true ? vlogd($(ok)) : vloge($(ok)));
    }
    {
        bool ok = sql.close_db();
        (ok == true ? vlogd($(ok)) : vloge($(ok)));
    }
}

int main()
{
    Tvlogs::get()->set_level(vlevel4::e_info);

    int ret = 2;
    if(ret == 1) test_1();
    if(ret == 2) test_2();


    vlogd("== end ==");

    return 0;
}
