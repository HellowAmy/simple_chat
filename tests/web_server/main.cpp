#include <iostream>

#include "inter_server.h"
#include "../../Tvlog.h"

using namespace std;

int main()
{
    Tvlogs::get()->set_level(vlevel4::e_info);
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
        if(msg == "close") sk.get_sv()->stop();
    };
    function fn_close = [&](const sp_channe&){
        vlogi("fn_close");
    };

    sk.func_bind(fn_open,fn_message,fn_close);
    sk.open();

    vlogi("== end ==");

    return 0;
}
