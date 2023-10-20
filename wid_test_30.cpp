#include "wid_test.h"

#include "Tvlog.h"
#include "qframe_line.h"
#include "wid_friend_info.h"
#include "typedef_struct.h"
#include "qmove_pos.h"
#include "qbutt_line.h"
#include "wid_person_info.h"
#include "wid_dialog_box.h"

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
//    qbutt_line *bu = new qbutt_line(this);
//    bu->move(400,100);t
    this->resize(1200,800);

    {
        wid_person_info *in = new wid_person_info(this);
        in->set_info({123456789,110122,19,1,"你好","中国广西"});
//        in->set_info_remarks("埃及和三等奖和");

        in->init_edit();
        //    in->init_show();
        in->show();
    }
    {
        wid_person_info *in = new wid_person_info(this);
        in->move(400,400);
        in->set_info({123456789,110122,19,1,"你好","中国广西"});
        in->set_info_remarks("埃及和三等奖和","../pic/icon2.jpg");

//        in->init_edit();
        in->init_show();
        in->show();
    }


//    connect(in,&wid_person_info::sn_change_info,[=](bool save,bool remarks){
//        vlogd($(save) $(remarks));

//        if(save)
//        {
//            if(remarks)
//            {
//                QString str = in->get_edit_remarks();
//                vlogd($(qstos(str)));
//            }
//            else
//            {
//                auto ct = in->get_edit_info();
//                vlogd($(ct.account)  $(ct.phone) $(ct.age) $(ct.sex)
//                      $(ct.location) $(ct.nickname) $(ct.icon));
//            }
//        }
//    });
}

void wid_test::test_31(QWidget *parent)
{
    //    QSize max_show = QSize(500,500);
    //    QSize max_info = QSize(240,140);
    //    QString file = "/home/red/open/github/simple_chat/pic/icon1.jpg";

    //    wid_friend_info *wid = new wid_friend_info(this);
    //    wid->resize(max_info);
    //    wid->set_name("Pajhisd");
    //    wid->set_icon(file);
    //    wid->init_info();

    //    qframe_line *wex = new qframe_line(this);
    //    wex->move(QPoint(wid->width() + 10,0));
    //    wex->resize(max_show);

    //    wid->set_extend_wid(wex);
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

