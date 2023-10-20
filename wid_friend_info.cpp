#include "wid_friend_info.h"

wid_friend_info::wid_friend_info(QWidget *parent)
    : qframe_line(parent)
{

    _space = 5;
    QSize max_info = QSize(240,140);
    this->resize(max_info);

    QSize size_icon(100,100);
    QSize size_butt(30,30);
    QSize size_extend(150,0);
    QSize size_extend_butt(size_extend.width() - _space*2,30);
    QSize size_name(this->width(),30);




    _butt_extend = new qbutt_line(this);

    //头像
    _wid_icon = new qframe_line(this);
    _wid_img = new qlab_img(_wid_icon);
    _wid_img->set_click(true);
//    _wid_img->set_img(_icon);
    _wid_img->set_keep(false,_wid_icon->calc_size(size_icon,_space));
    _wid_img->update_img();
    _wid_icon->resize_center(_wid_img,size_icon,_space);

    //名称
    _butt_name = new qbutt_line(this);
    _butt_name->resize(size_name);
    _butt_name->move(0,this->height() - _butt_name->height());
//    _butt_name->set_text(_name);



    _wid_extend = new qframe_line(this);
    _wid_extend->hide();

    _wid_person = new wid_person_info(this);
    _wid_person->init_edit();
    _wid_person->hide();

    //加入到显示管理
    _vec_extend.push_back(_wid_extend);
    _vec_extend.push_back(_wid_person);


    //扩展
    _butt_extend->resize(size_butt);
    _butt_extend->move(this->width() - _butt_extend->width()- _space,_space);
    _butt_extend->set_text(">>");

    QVector<QString> vec_extend_str {
                                "个人资料",
                                "搜索好友",
                                "添加好友",
                                "分组管理",
                                "切换账号",
                                "设置",
                                "退出"
    };


    //扩展区按钮
    _wid_extend->resize(size_extend);
    {
        int heigh = 0;
        qmove_pos move;
        for(auto &a:vec_extend_str)
        {
            qbutt_line *butt = new qbutt_line(_wid_extend);
            butt->resize(size_extend_butt);
            butt->set_text(a);
            butt->show();
            move.add_wid(butt);
            heigh += size_extend_butt.height() + _space;

            connect(butt,&qbutt_line::sn_clicked,this,[=](){
                emit sn_click_extend(butt->get_text());
            });
        }
        move.move_y(QPoint(_space,_space),_space);
        _wid_extend->resize(size_extend.width(),heigh + _space);
    }

    //名称按钮
    connect(_butt_name,&qbutt_line::sn_clicked,this,[=](){
        emit sn_click_name(_butt_name->get_text());
    });

    //点击扩展
    connect(_butt_extend,&qbutt_line::sn_clicked,this,[=](){
        show_wid_extend(_wid_extend);
        emit sn_show_extend();

//        if(_wid_extend->isHidden())
//        {

//            emit sn_show_extend(true);
//        }
//        else
//        {
//            show_wid_extend(nullptr);
//            emit sn_show_extend(false);
//        }
    });

    //扩展功能
    connect(this,&wid_friend_info::sn_click_extend,this,[=](QString tips){
        if(tips == "个人资料")
        {
            emit sn_info_all();
            show_wid_extend(_wid_person);
        }
    });

    //退出个人信息修改
    connect(_wid_person,&wid_person_info::sn_not_change,[=](){
        show_wid_extend(_wid_extend);
    });




    //更改头像
    connect(_wid_img,&qlab_img::sn_clicked,[=](){

        QString file = QFileDialog::getOpenFileName(this,"Open File","../pic/");

        if(is_img(file)) emit sn_change_icon(file);


////        QPixmap::isQBitmap()
////        QFileInfo::isRelative()

//        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
//                                                        "/home",
//                                                        tr("Images (*.png *.xpm *.jpg)"));

//        show_wid_extend(_wid_extend);
    });

}

void wid_friend_info::set_icon(QString icon)
{
    _wid_img->set_img(icon);
    _wid_img->update_img();
}

void wid_friend_info::set_name(QString name)
{
    _butt_name->set_text(name);
}

void wid_friend_info::show_extend()
{
    show_wid_extend(_wid_extend);
}

void wid_friend_info::set_extend_wid(QWidget *parent)
{
    _wid_extend->setParent(parent);
    _wid_person->setParent(parent);
}

wid_person_info *wid_friend_info::get_person()
{
    return _wid_person;
}

void wid_friend_info::status_extend(int status, QWidget *wid)
{
    if(status == 0)
    {
        _wid_extend->show();
        show_wid_extend(nullptr);
    }
    else if(status == 1)
    {

    }

}

void wid_friend_info::show_wid_extend(QWidget *wid)
{
    for(auto a:_vec_extend)
    {
        if(wid && wid == a) a->show();
        else a->hide();
    }
}

bool wid_friend_info::is_img(QString file)
{
    QImageReader img(file);
    return img.canRead();
}

//void wid_friend_info::set_extend(const QVector<QString> &vec)
//{
////    _vec_extend = vec;
//}

void wid_friend_info::init_info()
{

}

//void wid_friend_info::update_info()
//{
////    _wid_img->set_img(_icon);
////    _wid_img->update_img();
//}

//QWidget *wid_friend_info::get_extend_wid()
//{
//    return _wid_extend;
//}
