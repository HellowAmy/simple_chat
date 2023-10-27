#include "wid_test.h"

#include "Tvlog.h"
#include "qframe_line.h"
#include "wid_friend_info.h"
#include "typedef_struct.h"
#include "qmove_pos.h"
#include "qbutt_line.h"
#include "wid_person_info.h"
#include "wid_dialog_box.h"
#include "wid_friend_add.h"

#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <string>
#include <QDialog>
#include <tuple>

using namespace std;
using namespace typedef_struct;

void wid_test::test_30(QWidget *parent)
{
    this->resize(1200,800);

    {
        wid_person_info *in = new wid_person_info(this);
        in->set_info({123456789,110122,19,1,"你好","中国广西"});
        in->init_edit();
        in->show();
    }
    {
        wid_person_info *in = new wid_person_info(this);
        in->move(400,0);
        in->set_info({123456789,110122,19,1,"你好","中国广西"});
        in->set_info_remarks("埃及和三等奖和","../pic/icon2.jpg");
        in->init_friends();
        in->show();
    }
    {
        wid_person_info *in = new wid_person_info(this);
        in->move(0,400);
        in->init_add_ask();

        in->set_info({123456789,110122,19,1,"你好","中国广西"});
        in->set_info_remarks("","../pic/icon2.jpg");


        in->update_info();
        in->show();
        qlog(in->size());

        connect(in,&wid_person_info::sn_add_friend,[=](ct_ac_info ct,string remarks,string notes){
            vlogi($(ct.account) $(ct.age) $(ct.icon) $(ct.location)
                  $(ct.nickname) $(ct.phone) $(ct.sex));
            vlogi($(remarks) $(notes));
        });
    }

    {
        wid_person_info *in = new wid_person_info(this);
        in->move(600,400);
        in->init_add_recv();

        in->set_info({123456789,110122,19,1,"你好","中国广西"});
        in->set_info_remarks("","../pic/icon2.jpg","阿含经比vsd");

        in->update_info();
        in->show();
        qlog(in->size());

        connect(in,&wid_person_info::sn_save,[=](bool save){
            auto ct = in->get_edit_info();
            vlogi($(save) $(ct.account));
        });
    }
}

void wid_test::test_31(QWidget *parent)
{
    QSize max_show = QSize(500,500);
    this->resize(max_show);
    wid_frame(this);

    wid_friend_add *in = new wid_friend_add(this);

    in->get_person()->set_info({123456789,110122,19,1,"你好","中国广西"});
    in->get_person()->set_info_remarks("","../pic/icon2.jpg");
    in->get_person()->update_info();
    in->show();


    connect(in->get_person(),&wid_person_info::sn_add_friend,[=](ct_ac_info ct,string remarks,string notes){
        vlogi($(ct.account) $(ct.nickname) $(remarks) $(notes));
    });
}

void wid_test::test_32(QWidget *parent)
{

    qbutt_line *po = new qbutt_line(this);
    po->set_text("123123123");


    this->resize(100,100);
//    wid_dialog_box s1(this);
////    s1.resize(300,300);
//    bool ok = s1.make_box("","阿斯变化较大速度",wid_dialog_box::e_error);

    {
        bool ok = wid_dialog_box().make_box("提示","阿斯变化较大速度");
        vlogd($(ok));
    }
    {
        bool ok = wid_dialog_box().make_box("sadg","sdhfgsdfsdgf",wid_dialog_box::e_debug);
        vlogd($(ok));
    }
    {
        bool ok = wid_dialog_box().make_box("sadg","sdhfgsdfsdgf",wid_dialog_box::e_warning);
        vlogd($(ok));
    }
    {
        bool ok = wid_dialog_box().make_box("广泛","地方科技嗯",wid_dialog_box::e_error);
        vlogd($(ok));
    }


//    QDialog po;
//    po.setModal(true);
//    po.setResult(5);
//    po.exec();

//    vlogd($(po.result()));
}

