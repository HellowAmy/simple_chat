
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

#include <QTimer>
#include <QWidget>
#include <QApplication>

#include "../../qweb_client.h"
//#include "../../qweb_socket.h"
#include "../../Tvlog.h"

using namespace std;

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
//    a.setStyle(QStyleFactory::create("Fusion"));

//    wid_test w;
    //    w.show();





    Tvlogs::get()->set_level(vlevel4::e_info);

    vlogi("== client ==");

    bool loop = true;
    qweb_client ck;

    ck.connect(&ck,&qweb_client::sn_open,[&](){
        qlog("sn_open");
    });

    ck.connect(&ck,&qweb_client::sn_message,[=](string msg){
        qlog(msg);
    });

    ck.connect(&ck,&qweb_client::sn_close,[&](){
        loop = false;
        qlog("sn_close");
    });

    ck.open();

    QWidget w;
    QTimer ti;
    ti.connect(&ti,&QTimer::timeout,&w,[&](){

        vlogi("ttt");
        {
            bool ok = ck.send_sjson("sjson:ppppppppppppppppptttt");
            vlogi($(ok));
        }
    });
    ti.start(500);


    QTimer::singleShot(2600,&ti,[&](){
        ck.close_work();
        vlogi("singleShot");
    });



//    string str;
//    while(loop)
//    {
//        std::cin>>str;
//        ck.get_wc()->send(str);
//    }

//    ck.close_work();


//    ck.get_wc()->closesocket();
//    ck.get_wc()->close();
    vlogi("== end ==");

//    return 0;
    return a.exec();
}
