#include "wid_friend_info.h"

wid_friend_info::wid_friend_info(QWidget *parent)
    : qframe_line(parent)
{
    _space = 5;
    _butt_name = new qbutt_line(this);
    _butt_extend = new qbutt_line(this);
    _wid_icon = new qframe_line(this);
    _wid_img = new qlab_img(_wid_icon);
    _wid_extend = new qframe_line(this);
    _wid_extend->hide();

    connect(_butt_extend,&qbutt_line::sn_clicked,this,[=](){
        if(_wid_extend->isHidden())
        {
            _wid_extend->show();
            emit sn_show_extend(true);
        }
        else
        {
            _wid_extend->hide();
            emit sn_show_extend(false);
        }
    });
}

void wid_friend_info::set_icon(QString icon)
{
    _icon = icon;
}

void wid_friend_info::set_name(QString name)
{
    _name = name;
}

void wid_friend_info::set_extend_wid(QWidget *parent)
{
    _wid_extend->setParent(parent);
}

//void wid_friend_info::set_extend(const QVector<QString> &vec)
//{
////    _vec_extend = vec;
//}

void wid_friend_info::init_info()
{
    QSize size_icon(100,100);
    QSize size_butt(30,30);
    QSize size_extend(150,0);
    QSize size_extend_butt(size_extend.width() - _space*2,30);
    QSize size_name(this->width(),30);

    //头像
    _wid_img->set_img(_icon);
    _wid_img->set_keep(false,_wid_icon->calc_size(size_icon,_space));
    _wid_img->update_img();
    _wid_icon->resize_center(_wid_img,size_icon,_space);

    //名称
    _butt_name->resize(size_name);
    _butt_name->move(0,this->height() - _butt_name->height());
    _butt_name->set_text(_name);

    //扩展
    _butt_extend->resize(size_butt);
    _butt_extend->move(this->width() - _butt_extend->width()- _space,_space);
    _butt_extend->set_text(">>");

    QVector<QString> vec_extend {
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
        for(auto &a:vec_extend)
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
}

void wid_friend_info::update_info()
{
    _wid_img->set_img(_icon);
    _wid_img->update_img();
}

//QWidget *wid_friend_info::get_extend_wid()
//{
//    return _wid_extend;
//}
