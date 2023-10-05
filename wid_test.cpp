#include "wid_test.h"
#include "Tvlog.h"
#include "qbutt_line.h"
#include "qlab_line.h"
#include "qframe_line.h"
#include "qlab_wrap.h"
#include "qlab_img.h"
#include "wid_chat_display.h"
#include "qlab_bubble.h"
#include "wid_message.h"
#include "wid_msg_abs.h"
#include "wid_chat_output.h"
#include "wid_chat_input.h"
#include "wid_chat.h"
#include "wid_chat_list.h"
#include "wid_friend_butt.h"
#include "wid_friend_list.h"
#include "wid_friend_info.h"
#include "qsqlist.h"
#include "include/nlohmann/json.hpp"
#include "make_json.h"
#include "qweb_client.h"
#include "web_protocol.h"
#include "swap_files.h"
#include "qweb_files.h"

#include <filesystem>

#include <unistd.h>

#include <QTimer>
#include <QDateTime>
#include <QLabel>
#include <QFontMetrics>
#include <QString>
#include <QDebug>

void wid_frame(QWidget *wid)
{
    QLabel *lab = new QLabel(wid);
    lab->setAttribute(Qt::WA_TransparentForMouseEvents);
    lab->resize(wid->size());
    lab->setFrameShape(QFrame::Box);
    lab->show();
}

wid_test::wid_test(QWidget *parent)
    : QWidget(parent)
{
    this->show();
    {
        this->resize(750,500);
//        this->setFixedSize(750,500);
//        wid_frame(this);
    }

    Tvlogs::get()->set_level(vlevel4::e_info);
    vlogd("== begin ==");


    int val = 21;

    if(val == 1)        test_1(parent); //线条按钮
    else if(val == 2)   test_2(parent); //自动换行
    else if(val == 3)   test_3(parent); //线框文字
    else if(val == 4)   test_4(parent); //显示图片
    else if(val == 5)   test_5(parent); //线框图片
    else if(val == 6)   test_6(parent); //组合消息
    else if(val == 7)   test_7(parent); //聊天输出
    else if(val == 8)   test_8(parent); //聊天输入
    else if(val == 9)   test_9(parent); //聊天界面

    else if(val == 10)   test_10(parent);   //好友按钮
    else if(val == 11)   test_11(parent);   //好友列表
    else if(val == 12)   test_12(parent);   //信息区域
    else if(val == 13)   test_13(parent);   //测数据库
    else if(val == 14)   test_14(parent);   //测历史库
    else if(val == 15)   test_15(parent);   //网络连接
    else if(val == 16)   test_16(parent);   //传输格式
    else if(val == 17)   test_17(parent);   //格式生成
    else if(val == 18)   test_18(parent);   //发送测试
    else if(val == 19)   test_19(parent);   //协议统一

    else if(val == 20)   test_20(parent);   //文件测试
    else if(val == 21)   test_21(parent);   //文件传输


    vlogd("== end ==");
}

wid_test::~wid_test()
{
}

void wid_test::test_1(QWidget *parent)
{
    auto fn_butt = [=](QSize size, QPoint pos ,QString txt){
        qbutt_line *bt = new qbutt_line(this);
        bt->show();
        bt->resize(size);
        bt->move(pos);
        bt->set_text(txt);

        connect(bt,&qbutt_line::sn_clicked,this,[=](){
            vlogi(Q$(bt->get_text()));
        });
        return bt;
    };

    auto p1 = fn_butt({50,30},{0,0},"butt1");
    fn_butt({100,30},{0,100},"butt2");
    fn_butt({200,60},{0,200},"butt3");
    fn_butt({60,60},{0,300},"butt4");

    p1->set_col(Qt::darkRed);
    p1->keep_status(true);

    QTimer::singleShot(3000,this,[=](){
        p1->set_col(Qt::darkGray);
        p1->keep_status(false);
    });
}

void wid_test::test_2(QWidget *parent)
{
    auto fn_lab = [=](int width, QPoint pos ,QString txt){
        qlab_wrap *lab = new qlab_wrap(this);
        lab->set_text(txt);
        lab->set_width_max(width);
        lab->set_frame(true);
        lab->move(pos);
        lab->update_text();
    };

    {
        QString txt = "hellow world";

        fn_lab(150,{10,10},txt);
        fn_lab(150,{10,100 * 1 + 10},txt+txt);
        fn_lab(150,{10,100 * 2 + 10},txt+txt+txt+txt+txt+txt+txt+txt);
    }

    {
        QString txt = "hellow\nworld";

        fn_lab(150,{200,10},txt);
        fn_lab(150,{200,100 * 1 + 10},txt+txt);
        fn_lab(150,{200,100 * 2 + 10},txt+txt+txt+txt+txt+txt);
    }
}

void wid_test::test_3(QWidget *parent)
{
    QString txt = "hasdasdjhsdfsd";

    auto fn_txt = [=](int width, QPoint pos ,QString txt){
        int padding = 5;
        qframe_line *lab = new qframe_line(this);
        qlab_wrap *lab_txt = new qlab_wrap(lab);
        lab_txt->set_width_max(width);
        lab_txt->set_text(txt);
        lab_txt->update_text();
        lab->move(pos);
        lab->move_center(lab_txt,padding);
        lab->show();
        lab_txt->show();
    };

    fn_txt(150,{10,10},txt);
    fn_txt(150,{10,10 + 100 * 1},txt+txt);
//    fn_txt(150,{10,10 + 100 * 2},txt+txt+txt+txt+txt);

    QTimer::singleShot(1000,this,[=](){
        fn_txt(150,{10,10 + 100 * 2},txt+txt+txt+txt+txt);
    });
}

void wid_test::test_4(QWidget *parent)
{
    this->resize(1000,1000);
    auto fn_img = [=](QPoint pos,QSize size,QString txt,bool keep = true){
        qlab_img *lab = new qlab_img(this);
        lab->set_img(txt);
        lab->set_keep(keep,size);
        lab->move(pos);
        lab->update_img();
        lab->show();
    };

    fn_img({10,10},{0,0},"../pic/head.png");
    fn_img({10,10 + 250 * 1},{40,40},"../pic/head.png");
    fn_img({500,10},{0,0},"../pic/send.jpg");
    fn_img({10, 300},{400,400},"../pic/send.jpg");
    fn_img({10, 600},{200,200},"../pic/send.jpg",false);
}

void wid_test::test_5(QWidget *parent)
{
    auto fn_img_head = [=](QPoint pos,QSize size,QString txt,QColor col = Qt::darkGray){
        int padding = 5;
        qframe_line *lab_frame = new qframe_line(this);
        lab_frame->set_col(col);
        qlab_img *lab = new qlab_img(lab_frame);
        lab->set_img(txt);
        lab->set_keep(false,size);
        lab->update_img();
        lab_frame->move(pos);
        lab_frame->move_center(lab,padding);
        return lab_frame;
    };

    auto fn_img_body = [=](QPoint pos,QSize size,QString txt){
        int padding = 5;
        qframe_line *lab_frame = new qframe_line(this);
        qlab_img *lab = new qlab_img(lab_frame);
        lab->set_img(txt);
        lab->set_keep(true,size);
        lab->update_img();
        lab_frame->move(pos);
//        lab_frame->move_center(lab,padding);
        lab_frame->resize_center(lab,lab->size(),padding);
        qlog(lab_frame->size());
    };

    auto p1 = fn_img_head({10,10},{40,40},"../pic/head.png");
    fn_img_head({10,10 + 100 * 1},{40,40},"../pic/send.jpg",Qt::darkGreen);
    fn_img_body({10 + 100,10},{0,0},"../pic/head.png");
    fn_img_body({10,10 + 120 * 2},{370,370},"../pic/send.jpg");

    QTimer::singleShot(1000,this,[=](){
        p1->set_col(Qt::darkRed);
//        qlog("singleShot");
    });

}

void wid_test::test_6(QWidget *parent)
{
    this->resize(1200,1000);
    QWidget *wid_p = new QWidget(this);
    wid_p->resize(this->size());
    wid_p->show();

    QString str_date = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    long long date = QDateTime::currentDateTime().toMSecsSinceEpoch();
    QString txt = "hellow world! you user is python ?";

    auto fn_msg_text = [=](QPoint pos, QString txt,Qt::AlignmentFlag flg){
        wid_message *wid = new wid_message;
        wid->make_msg(new wid_msg_text(txt),"../pic/head.png",date,flg);
        wid->move(pos);

        wid->setParent(wid_p);
        wid->show();
    };

    auto fn_msg_date = [=](QPoint pos, QString txt,Qt::AlignmentFlag flg){
        wid_message *wid = new wid_message;
        wid->make_msg(new wid_msg_hint(txt),"",date,flg);
        wid->move(pos);

        wid->setParent(wid_p);
        wid->show();
    };

    auto fn_msg_img = [=](QPoint pos, QString txt,Qt::AlignmentFlag flg){
        wid_message *wid = new wid_message;
        wid->make_msg(new wid_msg_img(txt),"../pic/send.jpg",date,flg);
        wid->move(pos);

        wid->setParent(wid_p);
        wid->show();
    };

    fn_msg_text({0,0},txt,Qt::AlignRight);
    fn_msg_text({0,60},txt,Qt::AlignLeft);
//    fn_msg_text({0,150},txt+txt+txt,Qt::AlignRight);
//    fn_msg_text({0,250},txt+txt+txt,Qt::AlignLeft);

//    fn_msg_date({0,350},date,Qt::AlignLeft);
//    fn_msg_date({0,400},"对方撤回信息",Qt::AlignLeft);

//    fn_msg_img({510,0},"../pic/send.jpg",Qt::AlignRight);
//    fn_msg_img({510,250},"../pic/send.jpg",Qt::AlignLeft);
//    fn_msg_img({510,500},"../pic/head.png",Qt::AlignRight);


    QTimer::singleShot(1000,this,[=](){
        fn_msg_text({0,150},txt+txt+txt,Qt::AlignRight);
        fn_msg_text({0,250},txt+txt+txt,Qt::AlignLeft);

        fn_msg_date({0,350},str_date,Qt::AlignLeft);
        fn_msg_date({0,400},"对方撤回信息",Qt::AlignLeft);

        fn_msg_img({510,0},"../pic/send.jpg",Qt::AlignRight);
        fn_msg_img({510,250},"../pic/send.jpg",Qt::AlignLeft);
        fn_msg_img({510,500},"../pic/head.png",Qt::AlignRight);
    });

}

void wid_test::test_7(QWidget *parent)
{
    this->resize(1000,500);

    QString txt = "hellow world! you user is python ?";
    QString str_date = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    long long date = QDateTime::currentDateTime().toMSecsSinceEpoch();

    auto fn_msg = [=](wid_msg_abs *msg,Qt::AlignmentFlag flg){
        msg->show();
        wid_message *wid = new wid_message;
        wid->make_msg(msg,"../pic/head.png",date,flg);
        wid->show();
        return wid;
    };

    auto fn_hint = [=](wid_msg_abs *msg,Qt::AlignmentFlag flg){
        wid_message *wid = new wid_message;
        wid->make_msg(msg,"",date,flg);
        return wid;
    };

    wid_chat_output *chat = new wid_chat_output(this);
    chat->add_message(fn_msg(new wid_msg_text(txt,this),Qt::AlignRight));
    chat->add_message(fn_msg(new wid_msg_text(txt,this),Qt::AlignLeft));
    chat->add_message(fn_msg(new wid_msg_text(txt+txt+txt),Qt::AlignRight));
    chat->add_message(fn_msg(new wid_msg_text(txt+txt+txt),Qt::AlignRight));
//    chat->add_message(fn_msg(new wid_msg_text(txt+txt+txt),Qt::AlignLeft));
//    chat->add_message(fn_msg(new wid_msg_text(txt+txt+txt),Qt::AlignLeft));
//    chat->add_message(fn_msg(new wid_msg_img("../pic/send.jpg"),Qt::AlignRight));
//    chat->add_message(fn_msg(new wid_msg_img("../pic/send.jpg"),Qt::AlignLeft));
//    chat->add_message(fn_msg(new wid_msg_img("../pic/head.png"),Qt::AlignRight));
//    chat->add_message(fn_msg(new wid_msg_img("../pic/head.png"),Qt::AlignLeft));
    chat->add_message(fn_hint(new wid_msg_hint(str_date),Qt::AlignRight));
    chat->add_message(fn_hint(new wid_msg_hint("对方撤回消息"),Qt::AlignLeft));
//    chat->add_message(fn_msg(new wid_msg_text(txt+txt+txt),Qt::AlignRight));
//    chat->add_message(fn_msg(new wid_msg_text(txt+txt+txt),Qt::AlignLeft));

    QTimer::singleShot(1000,this,[=](){
        chat->add_message(fn_msg(new wid_msg_text(txt+txt+txt),Qt::AlignRight));
    });

    chat->init_size(QSize(500,500));
    chat->show();
}

void wid_test::test_8(QWidget *parent)
{
    wid_chat_input *wid = new wid_chat_input(this);
    wid->show();
}

void wid_test::test_9(QWidget *parent)
{
//    this->resize(1000,500);

    wid_chat *wid = new wid_chat(this);
    wid->get_input()->set_icon("../pic/two.png","../pic/one.png");
    wid->show();

    qlog(wid->size());
    qlog(wid->get_input()->size());
    qlog(wid->get_output()->size());

    QTimer::singleShot(1000,this,[=](){
        auto input = wid->get_input();
        emit input->sn_send_msg(input->make_text("123123",Qt::AlignLeft));
    });

    QTimer::singleShot(2000,this,[=](){
        auto input = wid->get_input();
        emit input->sn_send_msg(input->make_text("123123",Qt::AlignLeft));
        emit input->sn_send_msg(input->make_text("12312gfasdasfjasdgasd3asdhgfasdasfjasdasdasdasdafgsgsg",Qt::AlignLeft));
    });

    QTimer::singleShot(2000,this,[=](){
        auto input = wid->get_input();
        emit input->sn_send_msg(input->make_img("../pic/send.jpg",Qt::AlignLeft));
    });

    QTimer::singleShot(3000,this,[=](){
        auto input = wid->get_input();
        emit input->sn_send_msg(input->make_text("123123",Qt::AlignLeft));
    });

    QTimer::singleShot(3000,this,[=](){
        auto input = wid->get_input();
        emit input->sn_send_msg(input->make_text("3",Qt::AlignLeft));
    });

    QTimer::singleShot(4000,this,[=](){
        auto input = wid->get_input();
        emit input->sn_send_msg(input->make_msg(new wid_msg_hint("退出登陆"),Qt::AlignCenter));
        emit input->sn_send_msg(input->make_img("../pic/head.png",Qt::AlignLeft));
        emit input->sn_send_msg(input->make_msg(new wid_msg_hint("对方撤回消息"),Qt::AlignCenter));
        emit input->sn_send_msg(input->make_text("pic[../pic/head.png]",Qt::AlignLeft));
    });
}

void wid_test::test_10(QWidget *parent)
{
    this->resize(500,600);

    auto fn_butt = [=](QSize size,QString txt){
        wid_friend_butt *wid = new wid_friend_butt(this);
        wid->set_info("../pic/one.png",txt);
        wid->init_size(size);
        return wid;
    };

    qmove_pos move;
    move.add_wid(fn_butt({240,50},"叶文洁"));
    move.add_wid(fn_butt({240,50},"大史"));
    move.add_wid(fn_butt({240,50},"章北海"));
    move.move_y(QPoint(0,0),5);

//    {
//        wid_friend_butt *wid = new wid_friend_butt(this);
//        wid->set_info("../pic/one.png","你好");
//        wid->init_size(QSize(300,50));
//    }


//    wid_chat_list *wid = new wid_chat_list(this);
//    wid->resize(250,500);
//    wid->show();

}




void wid_test::test_11(QWidget *parent)
{
    this->resize(1000,600);

    QString name =
R"(汪淼
史强
常伟思
丁仪
杨冬
魏成
沙瑞山
徐冰冰
林云
李瑶
叶文洁
杨卫宁
雷志成
叶哲泰
邵琳
叶文雪
白沐霖
程丽华
阮雯
马钢
齐猎头
大凤
伊文斯
潘寒
申玉菲
拉菲尔)";

    QStringList ls = name.split("\n");
    ls.push_front("这是一个很长很长且没有什么意义的人取的名字");
    QVector<wid_friend_list::ct_info> vec;
    for(int i=0;i<ls.size();i++)
    {
        if(ls[i] != "")
        {
            if(i%4 == 0 && i != 0) vec.push_back({false,i,ls[i],"../pic/two.png"});
            else vec.push_back({true,i,ls[i],"../pic/one.png"});
        }
    }

    wid_friend_list *wid = new wid_friend_list(this);
    wid->set_icon("../pic/one.png");

    for(auto a:vec)
    {
        wid->add_friend(a);
    }

    this->resize(wid->size());
    qlog(wid->size());

    QTimer::singleShot(2000,this,[=](){
        wid->update_connect(0,false);
        wid->update_connect(1,false);
        wid->update_connect(2,false);
        wid->update_connect(3,false);
        wid->update_connect(4,false);
    });

    connect(wid,&wid_friend_list::sn_send_msg,this,[=](wid_friend_list::ct_msg ct){
        qlog(ct.id);
        qlog(ct.time);
        QDateTime ti;
        ti.setMSecsSinceEpoch(ct.time);
        qlog(ti.toString());
        qlog("\n");
    });
}

void wid_test::test_12(QWidget *parent)
{
    QVector<QString> vec;
    for(int i=0;i<10;i++)
    {
        vec.push_back(QString::number(i));
    }

    wid_friend_info *wid = new wid_friend_info(this);
    wid->resize(QSize(240,140));
    wid->set_icon("../pic/one.png");
    wid->set_name("帕斯的");
    wid->set_extend(vec);
    wid->update_info();

    connect(wid,&wid_friend_info::sn_click_extend,this,[=](bool extend,QString txt){
        qlog(txt);
    });

    connect(wid,&wid_friend_info::sn_click_name,this,[=](QString txt){
        qlog(txt);
    });
}

void wid_test::test_13(QWidget *parent)
{
    this->hide();

    QString test_db = "test_wid.db";
    QString test_table = "test_table";

    qsqlist_base li;
    {
        int ret = li.open_db(test_db);
        qlog("open_db: "<<ret);
    }

    {
        int ret = li.create_test(test_table);
        if(ret != 0) qlog(li.get_error_exec());
        qlog("create_db: "<<ret);
    }

    {
        int ret = li.insert_test(test_table,{1,"ab"});
        if(ret != 0) qlog(li.get_error_exec());
        qlog(ret);
    }
    {
        int ret = li.insert_test(test_table,{2,"abc"});
        if(ret != 0) qlog(li.get_error_exec());
        qlog(ret);
    }
    {
        int ret = li.insert_test(test_table,{3,"abcd"});
        if(ret != 0) qlog(li.get_error_exec());
        qlog(ret);
    }
    {
        int ret = li.insert_test(test_table,{3,"abcdtwo"});
        if(ret != 0) qlog(li.get_error_exec());
        qlog(ret);
    }

    {
        QVector<tuple<int, string>> vec;
        int ret = li.select_test(test_table,vec);

        qlog("select_db: "<<ret);
        for(auto a:vec)
        {
            qlog(std::get<0>(a) << std::get<1>(a));
        }
    }

    {
        int ret = li.delete_db(test_table);
        qlog("delete_db: "<<ret);
    }

    {
        int ret = li.drop_db(test_table);
        if(ret != 0) qlog(li.get_error_exec());
        qlog("drop_db: "<<ret);
    }

    {
        int ret = li.close_db();
        qlog("close_db: "<<ret);
    }
}

void wid_test::test_14(QWidget *parent)
{
    uint account = 123456789;
    uint friends = 123344556;

    qsqlist_history sql;
    {
        int ret = sql.open_history(account);
        qlog("open_history: "<<ret);
    }

    {
        int ret = sql.create_history(friends);
        qlog("create_history: "<<ret);
    }

    {
        int ret = sql.insert_history(friends,{12345,12345,0,2,2,"aghusdhasd","NULL"});
        if(ret != 0) qlog("insert_history: "<<sql.get_error_exec());
        qlog("insert_history: "<<ret);
    }
    {
        int ret = sql.insert_history(friends,{12346,123499,0,1,0,"aghusdhasd111","hellow"});
        if(ret != 0) qlog("insert_history: "<<sql.get_error_exec());
        qlog("insert_history: "<<ret);
    }
    {
        int ret = sql.insert_history(friends,{12347,123459,0,1,3,"aghusd222hasd","word"});
        if(ret != 0) qlog("insert_history: "<<sql.get_error_exec());
        qlog("insert_history: "<<ret);
    }
//    {
//        for(int i=0;i<300;i++)
//        {
//            sql.insert_history({i,1000,1,1,3,"aghusd222hasd"});
//        }
//        for(int i=0;i<700;i++)
//        {
//            sql.insert_history({i+300,1000,2,1,3,"aghusd222hasd"});
//        }
//    }

    {
        QString table("history_%1");
        table = table.arg(friends);
        QVector<QString> vec;
        int ret = sql.select_db(table,vec);
        qlog("select_db: "<<ret);
        for(auto a:vec)
        {
            qlog(a);
        }
    }

    {
        int ret = sql.update_history(friends,12345,sql.get_data().feed_back,12399);
        qlog("update_history: "<<ret);
    }
    {
        int ret = sql.update_history(friends,12345,sql.get_data().non_read,1);
        qlog("update_history: "<<ret);
    }
    {
        int ret = sql.update_history(friends,12345,sql.get_data().types,0);
        qlog("update_history: "<<ret);
    }
    {
        int ret = sql.update_history(friends,12345,sql.get_data().object,3);
        qlog("update_history: "<<ret);
    }
    {
        int ret = sql.update_history(friends,12345,sql.get_data().content,"aaabbb");
        qlog("update_history: "<<ret);
    }
    {
        int ret = sql.update_history(friends,12345,sql.get_data().extend,"are you ok");
        qlog("update_history: "<<ret);
    }
    {
        int ret = sql.update_history(friends,12346,sql.get_data().non_read,1);
        qlog("update_history: "<<ret);
    }

    {
        QString table("history_%1");
        table = table.arg(friends);
        QVector<QString> vec;
        int ret = sql.select_db(table,vec);
        qlog("select_db: "<<ret);
        for(auto a:vec)
        {
            qlog(a);
        }
    }

    {
        int ret = sql.check_non_read(friends);
        qlog("check_non_read: "<<ret);
    }
    {
        QString table("history_%1");
        table = table.arg(friends);
        int ret = sql.drop_db(table);
        qlog("drop_db: "<<ret);
    }
    {
        QString table("history_%1");
        table = table.arg(account);
        int ret = sql.drop_db(table);
        qlog("drop_db: "<<ret);
        qlog("drop_db: "<<sql.get_error());
        qlog("drop_db: "<<sql.get_error_exec());
    }
    {
        qlog("close_db: "<<sql.get_error());
        int ret = sql.close_db();
//        if(ret != 0) qlog("close_db: "<<sql.get_error());
        qlog("close_db: "<<ret);
        qlog("close_db: "<<sql.get_error());
    }
}

void wid_test::test_15(QWidget *parent)
{
    this->hide();

    int v = 100000000;

    for(int i=v;i<v+5;i++)
    {
        qweb_client *th = new qweb_client;
        connect(th,&qweb_client::sn_open,this,[=](){
            vlogi("sn_open");
            th->ask_login(i,"123456");
        });
        connect(th,&qweb_client::sn_close,this,[=](){
            vlogi("sn_close");
        });

        {
            bool ok = th->open();
            vlogi(ok);
        }
    }
}

void wid_test::test_16(QWidget *parent)
{
    this->hide();

    nlohmann::json js = nlohmann::json::parse(
R"({
  "pi": 3.141,
  "happy": true,
  "name": "Niels",
  "nothing": null,
  "answer": {
    "everything": 42
  },
  "list": [1, 0, 2],
  "object": {
    "currency": "USD",
    "value": 42.99
  }
}

)");

    vlogi(js.dump(4));


    vlogi($(js["name"]) << $(js["pi"]) );
    js["name"] = "new name";
    js["pi"] = 123.44;
    js["new ip"] = 3.33;
    js["new ipaaa"] = 3.33;

    try
    {
        int hu = js["pi11"].get<int>();
    }
    catch(...)
    {
        vlogi("123 catch");
    }


//    vlogi($(hu));

    vlogi($(js["name"]) << $(js["pi"]) <<$(js["bb"]));

    vlogi(js.dump(4));
}

void wid_test::test_17(QWidget *parent)
{
//    make_json mk;

//    connect(&mk,&make_json::sn_send_json,this,[](std::string str){
//        json js = json::parse(str);

//        vlogi($(str));
//        vlogi($(js.dump(4)));
//    });

//    {
//        bool ok = mk.send_swap_json(123456789,333456789,"Text","你好");
//        qlog(ok);
//    }
//    {
//        bool ok = mk.send_swap_json(123456789,666666789,"Text","head");
//        qlog(ok);
//    }
//    {
//        bool ok = mk.send_swap_json(123456789,222226789,"Img","word");
//        qlog(ok);
//    }
}

void wid_test::test_18(QWidget *parent)
{


    qweb_client th;

}

void wid_test::test_19(QWidget *parent)
{
    using namespace protocol;

    {
        vlogi("\n");
        string sjson = set_login(123456789, "password123");
        string stream;
        string type;
        if(check_json(sjson,stream,type))
        {
            vlogi($(stream) $(type));
        }
    }
    {
        vlogi("\n");
        string sjson = set_swap_msg(123456789,222456789,20010404,"Text","hellow");
        int64 target;
        int64 source;
        if(check_swap(sjson,target,source))
        {
            vlogi($(target) $(source));
        }
    }
    {
        string s = set_ac_info_json(123,"nicknamesss","iconsss",true);
        int64 ac_friends;
        string nickname;
        string icon;
        bool online;
        if(get_ac_info_json(s,ac_friends,nickname,icon,online))
        {
            vlogi($(ac_friends) $(nickname) $(icon) $(icon));
        }
    }

    //== json ==
    {
        vlogi("\n");
        string sjson = set_login(123456789, "password123");

        int64 account;
        string passwd;
        if (get_login(sjson, account, passwd))
        {
            vlogi($(account) $(passwd));

            string ssjson = set_login_back(true);
            bool ok;
            if (get_login_back(ssjson, ok))
            {
                vlogi($(ok));
            }
        }
    }

    {
        vlogi("\n");
        string sjson = set_swap_msg(123456789,999956789,1000000,"Text","123456");
        vlogi($(sjson));

        string stream;
        string type;
        if(check_json(sjson,stream,type))
        {
            vlogi($(stream) $(type));

            int64 ac_to;
            int64 ac_from;
            int64 time;
            string type;
            string content;
            if(get_swap_msg(sjson,ac_to,ac_from,time,type,content))
            {
                vlogi($(ac_to) $(ac_from) $(time) $(type) $(content));
            }
        }
    }

    {
        vlogi("\n");
        string sjson = set_friends_list(123456789);

        int64 account;
        if(get_friends_list(sjson,account))
        {
            vlogi($(account));

            string ss = set_json_vec({"123","456","abc","999"});
            string sjson1 = set_friends_list_back(ss,true);

            string svec;
            bool ret;
            if(get_friends_list_back(sjson1,svec,ret))
            {
                vlogi($(svec) $(ret));

                vector<string> vec = get_json_vec(svec);
                for(auto a:vec)
                {
                    vlogi($(a));
                }
            }
        }
    }

    {
        vlogi("\n");
        string sjson = set_swap_msg(123456789,222456789,20010404,"Text","hellow");

        int64 target;
        int64 source;
        int64 time_to;
        string type;
        string content;

        if(get_swap_msg(sjson,target,source,time_to,type,content))
        {
            vlogi($(target) $(source) $(time_to) $(type) $(content));

            string ssjson = set_swap_msg_back(source,target,time_to,time_to+100,true);

            int64 target;
            int64 source;
            int64 time_to;
            int64 time_ok;
            bool ok;
            if(get_swap_msg_back(ssjson,target,source,time_to,time_ok,ok))
            {
                vlogi($(target) $(source) $(time_to) $(time_ok) $(ok));
            }
        }
    }

    {
        vlogi("\n");
        vector<string> vec;
        vec.push_back(set_ac_info_json(123,"nickname11","icon11",true));
        vec.push_back(set_ac_info_json(123456,"nickname22","icon22",false));
        vec.push_back(set_ac_info_json(999,"nickname33","icon33",true));

        string vec_s = set_json_vec(vec);

        string sjson = set_friends_status(vec_s);

        string vec_gs;
        if (get_friends_status(sjson,vec_gs))
        {
            vlogi($(vec_gs));
            auto vec = get_json_vec(vec_gs);

            for(auto a:vec)
            {
                vlogi($(a));

                int64 ac_friends;
                string nickname;
                string icon;
                bool online;
                if(get_ac_info_json(a,ac_friends,nickname,icon,online))
                {
                    vlogi($(ac_friends) $(nickname) $(icon) $(online));
                }
            }
        }
    }

    {
        vlogi("\n");
        string svec_ac_fs = "your_friends_status_data_here";


//        set_ac_info_json();
//        get_ac_info_json()
        string sjson = set_friends_status(svec_ac_fs);

        string svec_ac_info;
        if (get_friends_status(sjson, svec_ac_info))
        {
            vlogi(svec_ac_info);

            string ssjson = set_friends_status_back(svec_ac_info, true);

            bool ok;
            if (get_friends_status_back(ssjson, svec_ac_info, ok))
            {
                vlogi(svec_ac_info + " " + $(ok));
            }
        }
    }


    //== files ==
    vlogi("files ======================\n");

    {
        string s = set_files_json("{123456}");
        if(check_files_flg(s) == CS_FILES_JSON) vlogi("CS_FILES_JSON");
        else if(check_files_flg(s) == CS_FILES_BINARY) vlogi("CS_FILES_BINARY");

        string sjson;
        if(get_files_json(s,sjson))
        {
            vlogi($(sjson));
        }
    }

    {
        string b = "0x990x080x77";
        string s = set_files_binary(12344418739845834,b);
        vlogi($(s.size()) $(b.size()));

        if(check_files_flg(s) == CS_FILES_JSON) vlogi("CS_FILES_JSON");
        else if(check_files_flg(s) == CS_FILES_BINARY) vlogi("CS_FILES_BINARY");

        int64 id;
        string sjson;
        if(get_files_binary(s,id,sjson))
        {
            vlogi($(id) $(sjson));
        }
    }

    {
        vlogi("\n");
        int64 time = 123456789; // 替换为你的时间值
        int64 target = 987654321; // 替换为目标值
        int64 source = 555555555; // 替换为源值
        int64 length_max = 1024; // 替换为最大长度值
        string filename = "example.txt"; // 替换为文件名

        // 创建发送文件上传请求的 JSON 字符串
        string upload_json = set_files_create_upload(time, length_max, filename);

        int64 gtime;
        int64 glength_max;
        string gfilename;
        bool upload_ok;
        if (get_files_create_upload(upload_json,gtime,glength_max,gfilename))
        {
            vlogi($(gtime) $(glength_max) $(gfilename) $(upload_ok));
        }

        // 创建返回文件上传结果的 JSON 字符串
        string upload_back_json = set_files_create_upload_back(time, 12399999001, true);

        int64 upload_back_time;
        int64 upload_back_swap_name;
        bool upload_back_ok;
        if (get_files_create_upload_back(upload_back_json, upload_back_time, upload_back_swap_name, upload_back_ok))
        {
            vlogi($(upload_back_time) $(upload_back_swap_name) $(upload_back_ok));
        }
    }


    {
        vlogi("\n");
        string sjson = set_files_finish_upload(123456789, true);

        int64 swap_name;
        bool swap_data;
        bool finish;

        if (get_files_finish_upload(sjson, swap_name, finish))
        {
            vlogi($(swap_name) $(finish));

            string ssjson = set_files_finish_upload_back(swap_name, true);

            bool ok;
            if (get_files_finish_upload_back(ssjson, swap_name, ok))
            {
                vlogi($(swap_name) $(ok));
            }
        }
    }



    {
        vlogi("\n");
        int64 swap_name = 123456789; // 替换为你的交换名称值

        // 创建发送文件下载请求的 JSON 字符串
        string download_json = set_files_create_download(swap_name);

        int64 download_swap_name;
        if (get_files_create_download(download_json, download_swap_name))
        {
            vlogi($(download_swap_name));
        }

        // 创建返回文件下载请求结果的 JSON 字符串
        int64 id = 1024; // 替换为文件最大长度值
        int64 length_max = 1024; // 替换为文件最大长度值
        string filename = "downloaded_file.txt"; // 替换为下载的文件名
        string download_back_json = set_files_create_download_back(id,length_max, filename, true);

        int64 download_id;
        int64 download_back_length_max;
        string download_back_filename;
        bool download_back_ok;
        if (get_files_create_download_back(download_back_json,download_id, download_back_length_max, download_back_filename, download_back_ok))
        {
            vlogi($(download_back_length_max) $(download_back_filename) $(download_back_ok));
        }
    }

    {
        vlogi("\n");
        string sjson = set_files_begin_download(123456789,true);

        int64 swap_name;
        bool ok;
        if (get_files_begin_download(sjson, swap_name,ok))
        {
            vlogi($(swap_name));
        }
    }

    {
        vlogi("\n");
        string sjson = set_files_finish_download(123456789, true);

        int64 swap_name;
        bool ok;

        if (get_files_finish_download(sjson, swap_name, ok))
        {
            vlogi($(swap_name) $(ok));
        }
    }

    {
        vlogi("\n");
        string sjson = set_files_cancel_download(123456789);

        int64 swap_name;

        if (get_files_cancel_download(sjson, swap_name))
        {
            vlogi($(swap_name));

            string ssjson = set_files_cancel_download_back(swap_name, true);

            bool ok;
            if (get_files_cancel_download_back(ssjson, swap_name, ok))
            {
                vlogi($(swap_name) $(ok));
            }
        }
    }

    //== files ==

}

void wid_test::test_20(QWidget *parent)
{
    this->hide();

    int64 id = 123456789;
    string s1 = "/home/red/open/load/clion.png";
    string s2 = "/home/red/open/load/CLion202322.tar.gz";
    string s3 = "/home/red/open/load/none.txt";
    string ss = "/home/red/open/load/123456789";
    string ss1 = "/home/red/open/load/save_12345";

    swap_files s;
    {
        auto ok = s.is_can_read(s1);
        vlogi($(ok));
    }
    {
        auto ok = s.is_can_read(ss1);
        vlogi($(ok));
    }
    {
        auto ok = s.is_exists(s1);
        vlogi($(ok));
    }
    {
        auto ok = s.is_file(s1);
        vlogi($(ok));
    }
    {
        auto ok = s.get_file_size(s1);
        vlogi($(ok));
    }
    {
        auto ok = s.get_filename(s1);
        vlogi($(ok));
    }

//    return; //函数测试部分



//    //收发测试部分
//    auto fn = [&](bool &stop,int64 id,const string &buf){

//        bool ok = s.add_data_recv(id,buf);
//        if(ok == false) vlogi($(ok));

//        static size_t co = 0;
//        co += buf.size();
//        if(co%100000 == 0)
//        {
//            std::this_thread::sleep_for(std::chrono::milliseconds(50));
//            vlogi("sleep_for:" );
//        }
//    };

//    {
//        bool ok = s.open_file_recv(fn,id,s1);
//        vlogi($(ok));
//    }
//    {
//        s.close_file_recv(id);
//    }

}

void wid_test::test_21(QWidget *parent)
{
    this->hide();

#if 0
    string s1 = "/home/red/open/load/clion.png";
    string s2 = "/home/red/open/load/CLion202322.tar.gz";
    string s3 = "/home/red/open/load/none.txt";

    for(int i=0;i<5;i++)
    {
        int64 time = 123456789;
        int64 account = 1122334455;
        qweb_files *th = new qweb_files;
        connect(th,&qweb_files::sn_open,this,[=](){
            vlogi("sn_open");

            bool ok = th->upload_file(time+i,s1);
            vlogi($(ok));
        });

        connect(th,&qweb_files::sn_close,this,[=](){
            vlogi("sn_close");
        });

        th->open();
    }

#else

//    vector<int64> vec {490645973,751498503,167392342,822058707};
//    vector<int64> vec {822058707};

//    std::vector<int64> vec = {
//        167392342,
//        232304622,
//        239478879,
//        464308397,
//        490645973,
//        531637645,
//        751498503,
//        755361055,
//        822058707,
//        933347679
//    };

    std::vector<int64> vec = {
        232304622,
        464308397,
        531637645,
        755361055,
        822058707
    };

    qweb_files *th = new qweb_files;
    connect(th,&qweb_files::sn_open,this,[=](){
        vlogi("sn_open");

        for(int i=0;i<vec.size();i++)
        {
            bool ok = th->download_file(vec[i]);
            vlogi($(ok));
        }
    });

    connect(th,&qweb_files::sn_close,this,[=](){
        vlogi("sn_close");
    });
    th->open();


//    auto fn_th = [=](int64 id){
//        qweb_files *th = new qweb_files;
//        connect(th,&qweb_files::sn_open,this,[=](){
//            vlogi("sn_open");

//            bool ok = th->download_file(id);
//            vlogi($(ok));
//        });

//        connect(th,&qweb_files::sn_close,this,[=](){
//            vlogi("sn_close");
//        });

//        th->open();
//    };

//    for(int i=0;i<vec.size();i++)
//    {
//        try{
//            std::thread th(fn_th,vec[i]);
//            th.detach();
//        }catch(...){}

//    }
#endif



}


