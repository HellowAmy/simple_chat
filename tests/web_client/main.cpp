
//#include <QApplication>
//#include <QStyleFactory>

//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    a.setStyle(QStyleFactory::create("Fusion"));

//    wid_test w;
//    //    w.show();

//    return a.exec();
//}

#include <iostream>

#include "../../qweb_client.h"
//#include "../../qweb_socket.h"
#include "../../Tvlog.h"

using namespace std;

int main()
{
    Tvlogs::get()->set_level(vlevel4::e_info);

    vlogi("== client ==");

    bool loop = true;
    qweb_client ck;
//    function fn_open = [](){
//        qlog("ck-fn_open");
//    };
//    function fn_message = [](const string& msg){
//        qlog("ck-fn_message: " << msg);
//    };
//    function fn_close = [&](){
//        qlog("ck-fn_close");
//        loop = false;
//        ck.get_wc()->close();
//    };
//    ck.func_bind(fn_open,fn_message,fn_close);

    ck.open();

    ck.connect(&ck,&qweb_client::sn_message,[=](QString msg){
        qlog(msg);
    });

    ck.connect(&ck,&qweb_client::sn_close,[&](){
        loop = false;
        qlog("sn_close");
    });

    string str;
    while(loop)
    {
        std::cin>>str;
        ck.get_wc()->send(str);
    }

    vlogi("== end ==");

    return 0;
}
