#include "wid_friend_add.h"

wid_friend_add::wid_friend_add(QWidget *parent)
    : qframe_line(parent)
{
    int space = 5;
    QSize size_lab = QSize(60,30);
    QSize size_edit = QSize(180,30);

    _butt = new qbutt_line(this);
    _butt->set_text("查找");
    _butt->resize(size_lab);

    auto [a,b] = fn_Label(this,"账号",size_lab);
    auto [c,d] = fn_LineEdit_txt(this,"",size_edit);
    _edit = d;
    _edit->setValidator(new QIntValidator(this));

    //账号输入与按钮
    {
        qmove_pos move;
        move.add_wid(a);
        move.add_wid(c);
        move.add_wid(_butt);
        move.move_x({space,space},space);
    }

    _info = new wid_person_info(this);
    _info->init_add_ask();

    //显示信息的整体布局
    {
        qmove_pos move;
        move.add_wid(a);
        move.add_wid(_info);
        auto size = move.move_y({space,space},space*6);
        this->resize(size);
        _info->move(0,a->height()+space*6);
    }

    //
    connect(_butt,&qbutt_line::sn_clicked,this,[=](){
        sn_connect_t::get()->sn_extend_add_find
            (en_info_all::e_ask_ac,_edit->text().toLongLong());
    });

//    //
//    connect(_info,&wid_person_info::sn_add_friend,this,[=](ct_ac_info ct){
//        sn_connect_t::get()->sn_extend_add_find
//            (en_info_all::e_ask_ac,_edit->text().toLongLong());
//    });

    //
    connect(sn_connect_t::get(),&sn_connect::sn_extend_add_find_back,this,[=](ct_ac_info info){
        _info->set_info(info);

        //下载结果
        auto fn_cb = [=](bool ok,int64 account,string save_path){
            if(ok)
            {
                _info->set_info_remarks("",to_path_icon(account));
                _info->update_info();
            }
            else vlogw("download_icon failed");
        };

        //载入图片
        if(is_exists_icon(info.account) == false)
        { sn_connect_t::get()->download_icon(info.account,fn_cb); }
        else _info->set_info_remarks("",to_path_icon(info.account));

        //更新信息
        _info->update_info();
    });






//    //
//    connect(_info,&wid_person_info::sn_add_friend,this,[=](){
////        _info->set_info();
//        _info->update_info();
//        sn_connect_t::get()->sn_extend_add_find
//            (en_info_all::e_ask_ac,_edit->text().toLongLong());
//    });


//    //
//    connect(_butt,&qbutt_line::sn_clicked,this,[=](){
//        sn_connect_t::get()->sn_extend_add_find
//            (en_info_all::e_ask_ac,_edit->text().toLongLong());
//    });
}

wid_person_info *wid_friend_add::get_person()
{
    return _info;
}
