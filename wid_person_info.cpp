#include "wid_person_info.h"

#include "Tvlog.h"

wid_person_info::wid_person_info(QWidget *parent)
    : qframe_line(parent)
{
    _space = 5;
    _size_lab = QSize(60,30);
    _size_edit = QSize(180,30);


    QSize size_butt(120,40);
    _cancel = new qbutt_line(this);
    _cancel->set_text("取消");
    _cancel->resize(size_butt);

    _save = new qbutt_line(this);
    _save->set_text("确认");
    _save->resize(size_butt);

    _add = new qbutt_line(this);
    _add->set_text("添加");
    _add->resize(QSize(size_butt.width()*2 +_space,size_butt.height()));
    _add->hide();

    _img = nullptr;

    connect(_cancel,&qbutt_line::sn_clicked,[=](){
        emit sn_save(false);
    });

    connect(_save,&qbutt_line::sn_clicked,[=](){
        emit sn_save(true);
    });

    connect(_add,&qbutt_line::sn_clicked,[=](){
        emit sn_add_friend(get_edit_info(),get_edit_remarks(),get_edit_notes());
    });

    connect(this,&wid_person_info::sn_save,[=](bool save){
        if(save)
        {
            if(not_change_info() == false)
            {
                if(is_verification_info())
                {
                    bool ok = wid_dialog_box().make_box("确认修改","<< 个人信息以改变，请确认修改个人信息 >>",
                                wid_dialog_box::e_warning);
                    if(ok) emit sn_save_info(get_edit_info());
                    return ;
                }
                else
                {
                    wid_dialog_box().make_box("修改限制","性别：男 或的 女 \n年龄：0 至 200",
                        wid_dialog_box::e_warning);
                    return;
                }
            }
            else if(not_change_remarks() == false) { emit sn_save_remarks(get_edit_remarks()); }
        }
        emit sn_not_change();
    });
}

void wid_person_info::set_info(const ct_ac_info &info)
{
    QString sex;
    if(info.sex == 0) sex = "女";
    else sex = "男";

    _ct_save.account    = QString::number(info.account);
    _ct_save.phone      = QString::number(info.phone);
    _ct_save.age        = QString::number(info.age);
    _ct_save.sex        = sex;
    _ct_save.nickname   = stoqs(info.nickname);
    _ct_save.location   = stoqs(info.location);
}

void wid_person_info::set_info_remarks(QString remarks,QString icon,QString notes)
{
    if(remarks != "") _ct_save.remarks = remarks;
    if(icon != "") _ct_save.icon = icon;
    if(notes != "") _ct_save.notes = notes;
}

void wid_person_info::init_edit()
{
    qmove_pos move;
    {
        auto [a,b] = fn_Label(this,"账号",_size_lab);
        auto [c,d] = fn_LineEdit_non(this,_ct_save.account,_size_edit);

        _edit.account = d;
        move.add_wid(a);
        move.add_wid(c);
    }
    {
        auto [a,b] = fn_Label(this,"昵称",_size_lab);
        auto [c,d] = fn_LineEdit_txt(this,_ct_save.nickname,_size_edit);

        _edit.nickname = d;
        move.add_wid(a);
        move.add_wid(c);
    }
    {
        auto [a,b] = fn_Label(this,"电话",_size_lab);
        auto [c,d] = fn_LineEdit_txt(this,_ct_save.phone,_size_edit);

        _edit.phone = d;
        _edit.phone->setValidator(new QIntValidator);
        move.add_wid(a);
        move.add_wid(c);
    }
    {
        auto [a,b] = fn_Label(this,"年龄",_size_lab);
        auto [c,d] = fn_LineEdit_txt(this,_ct_save.age,_size_edit);

        _edit.age = d;
        _edit.age->setValidator(new QIntValidator);
        move.add_wid(a);
        move.add_wid(c);
    }
    {
        auto [a,b] = fn_Label(this,"性别",_size_lab);
        auto [c,d] = fn_LineEdit_txt(this,_ct_save.sex,_size_edit);

        _edit.sex = d;
        move.add_wid(a);
        move.add_wid(c);
    }
    {
        auto [a,b] = fn_Label(this,"地址",_size_lab);
        auto [c,d] = fn_LineEdit_txt(this,_ct_save.location,_size_edit);

        _edit.location = d;
        move.add_wid(a);
        move.add_wid(c);
    }

    move.add_wid(_cancel);
    move.add_wid(_save);

    auto size = move.move_group({_space,_space},_space,2,-1);
    this->resize(size + QSize(_space*2,_space*2));
}

void wid_person_info::init_friends()
{
    qmove_pos move;
    {
        auto [a,b] = fn_Label(this,"备注",_size_lab);
        auto [c,d] = fn_LineEdit_txt(this,_ct_save.remarks,_size_edit);

        _edit.remarks = d;
        move.add_wid(a);
        move.add_wid(c);
    }
    {
        auto [a,b] = fn_Label(this,"账号",_size_lab);
        auto [c,d] = fn_LineEdit_non(this,_ct_save.account,_size_edit);

        _edit.account = d;
        move.add_wid(a);
        move.add_wid(c);
    }
    {
        auto [a,b] = fn_Label(this,"昵称",_size_lab);
        auto [c,d] = fn_LineEdit_non(this,_ct_save.nickname,_size_edit);

        _edit.nickname = d;
        move.add_wid(a);
        move.add_wid(c);
    }
    {
        auto [a,b] = fn_Label(this,"电话",_size_lab);
        auto [c,d] = fn_LineEdit_non(this,_ct_save.phone,_size_edit);

        _edit.phone = d;
        move.add_wid(a);
        move.add_wid(c);
    }
    {
        auto [a,b] = fn_Label(this,"年龄",_size_lab);
        auto [c,d] = fn_LineEdit_non(this,_ct_save.age,_size_edit);

        _edit.age = d;
        move.add_wid(a);
        move.add_wid(c);
    }
    {
        auto [a,b] = fn_Label(this,"性别",_size_lab);
        auto [c,d] = fn_LineEdit_non(this,_ct_save.sex,_size_edit);

        _edit.sex = d;
        move.add_wid(a);
        move.add_wid(c);
    }
    {
        auto [a,b] = fn_Label(this,"地址",_size_lab);
        auto [c,d] = fn_LineEdit_non(this,_ct_save.location,_size_edit);

        _edit.location = d;
        move.add_wid(a);
        move.add_wid(c);
    }

    move.add_wid(_cancel);
    move.add_wid(_save);

    auto size = move.move_group({_space,_space},_space,2,-1);
    this->resize(size + QSize(_space*2,_space*2));

    {
        auto [a,b] = fn_fimg(this,_ct_save.icon,QSize(200 + 10,200 + 10));
        qframe_line *f = a;
        _img = b;
        f->move(this->width(),_space);
        this->resize(QSize(this->width() + f->width() + _space,this->height()));
    }
}

void wid_person_info::init_add_ask()
{
    qmove_pos move;
    {
        auto [a,b] = fn_Label(this,"账号",_size_lab);
        auto [c,d] = fn_LineEdit_non(this,_ct_save.account,_size_edit);

        _edit.account = d;
        move.add_wid(a);
        move.add_wid(c);
    }
    {
        auto [a,b] = fn_Label(this,"昵称",_size_lab);
        auto [c,d] = fn_LineEdit_non(this,_ct_save.nickname,_size_edit);

        _edit.nickname = d;
        move.add_wid(a);
        move.add_wid(c);
    }
    {
        auto [a,b] = fn_Label(this,"电话",_size_lab);
        auto [c,d] = fn_LineEdit_non(this,_ct_save.phone,_size_edit);

        _edit.phone = d;
        move.add_wid(a);
        move.add_wid(c);
    }
    {
        auto [a,b] = fn_Label(this,"年龄",_size_lab);
        auto [c,d] = fn_LineEdit_non(this,_ct_save.age,_size_edit);

        _edit.age = d;
        move.add_wid(a);
        move.add_wid(c);
    }
    {
        auto [a,b] = fn_Label(this,"性别",_size_lab);
        auto [c,d] = fn_LineEdit_non(this,_ct_save.sex,_size_edit);

        _edit.sex = d;
        move.add_wid(a);
        move.add_wid(c);
    }
    {
        auto [a,b] = fn_Label(this,"地址",_size_lab);
        auto [c,d] = fn_LineEdit_non(this,_ct_save.location,_size_edit);

        _edit.location = d;
        move.add_wid(a);
        move.add_wid(c);
    }
    {
        auto [a,b] = fn_Label(this,"好友备注",_size_lab);
        auto [c,d] = fn_LineEdit_txt(this,_ct_save.remarks,_size_edit);

        _edit.remarks = d;
        move.add_wid(a);
        move.add_wid(c);
    }
    {
        auto [a,b] = fn_Label(this,"申请信息",_size_lab);
        auto [c,d] = fn_qedit_txt(this,_size_edit + QSize(200 + _space*3,50));
        move.add_wid(a);
        move.add_wid(c);

        _edit.notes = d;
        _edit.notes->show();
        _edit.notes->setText("你好,如果想认识一下请交个朋友吧!");
    }

    _cancel->hide();
    _save->hide();
    _add->show();
    move.add_wid(_add);

    auto size = move.move_group({_space,_space},_space,2,-1);
    this->resize(size + QSize(_space*2,_space*2));

    {
        auto [a,b] = fn_fimg(this,_ct_save.icon,QSize(200 + _space*2,200 + _space*2));
        qframe_line *f = a;
        _img = b;
        f->move(_size_lab.width() + _size_edit.width() + _space*3,_space);
    }
}

void wid_person_info::init_add_recv()
{
    qmove_pos move;
    {
        auto [a,b] = fn_Label(this,"账号",_size_lab);
        auto [c,d] = fn_LineEdit_non(this,_ct_save.account,_size_edit);

        _edit.account = d;
        move.add_wid(a);
        move.add_wid(c);
    }
    {
        auto [a,b] = fn_Label(this,"昵称",_size_lab);
        auto [c,d] = fn_LineEdit_non(this,_ct_save.nickname,_size_edit);

        _edit.nickname = d;
        move.add_wid(a);
        move.add_wid(c);
    }
    {
        auto [a,b] = fn_Label(this,"电话",_size_lab);
        auto [c,d] = fn_LineEdit_non(this,_ct_save.phone,_size_edit);

        _edit.phone = d;
        move.add_wid(a);
        move.add_wid(c);
    }
    {
        auto [a,b] = fn_Label(this,"年龄",_size_lab);
        auto [c,d] = fn_LineEdit_non(this,_ct_save.age,_size_edit);

        _edit.age = d;
        move.add_wid(a);
        move.add_wid(c);
    }
    {
        auto [a,b] = fn_Label(this,"性别",_size_lab);
        auto [c,d] = fn_LineEdit_non(this,_ct_save.sex,_size_edit);

        _edit.sex = d;
        move.add_wid(a);
        move.add_wid(c);
    }
    {
        auto [a,b] = fn_Label(this,"地址",_size_lab);
        auto [c,d] = fn_LineEdit_non(this,_ct_save.location,_size_edit);

        _edit.location = d;
        move.add_wid(a);
        move.add_wid(c);
    }
    {
        auto [a,b] = fn_Label(this,"好友备注",_size_lab);
        auto [c,d] = fn_LineEdit_txt(this,_ct_save.remarks,_size_edit);

        _edit.remarks = d;
        move.add_wid(a);
        move.add_wid(c);
    }
    {
        auto [a,b] = fn_Label(this,"申请信息",_size_lab);
        auto [c,d] = fn_qedit_txt(this,_size_edit + QSize(200 + _space*3,50));

        _edit.notes = d;
        move.add_wid(a);
        move.add_wid(c);

        qframe_line* f = c;
        f->set_col(QColor(0xaa0000));

        _edit.notes->setFocusPolicy(Qt::NoFocus);
        _edit.notes->setText(_ct_save.notes);
    }

//    _cancel->hide();
//    _save->hide();
//    _add->show();
    _cancel->set_text("拒绝");
    _save->set_text("同意");
    move.add_wid(_cancel);
    move.add_wid(_save);

//    auto size = move.move_group({_space,_space},_space,2,-1);
//    this->resize(size + QSize(_space*2,_space*2));

    auto size = move.move_group({_space,_space},_space,2,-1);
    this->resize(size + QSize(_space*2,_space*2));

    {
        auto [a,b] = fn_fimg(this,_ct_save.icon,QSize(200 + _space*2,200 + _space*2));
        qframe_line *f = a;
        _img = b;
        f->move(_size_lab.width() + _size_edit.width() + _space*3,_space);
    }
}

bool wid_person_info::not_change_info()
{
    if(_ct_save.account     != _edit.account->text())   return false;
    if(_ct_save.phone       != _edit.phone->text())     return false;
    if(_ct_save.age         != _edit.age->text())       return false;
    if(_ct_save.sex         != _edit.sex->text())       return false;
    if(_ct_save.nickname    != _edit.nickname->text())  return false;
    if(_ct_save.location    != _edit.location->text())  return false;
    return true;
}

bool wid_person_info::not_change_remarks()
{
    if(_edit.remarks != nullptr)
    {
        if(_edit.remarks->text() == _ct_save.remarks) return true;
        else return false;
    }
    return true;
}

bool wid_person_info::is_verification_info()
{
    int64 age = _edit.age->text().toLongLong();
    if(_edit.phone->text().toLongLong() < 0) return false;
    else if((age < 0) || (age > 200)) return false;

    else if(_edit.sex->text() != "男"
             && _edit.sex->text() != "女"
             && _edit.sex->text() != "")
    { return false; }

    return true;
}

void wid_person_info::update_info()
{
    if(_edit.remarks) _edit.remarks->setText(_ct_save.remarks);
    if(_edit.notes) _edit.notes->setText(_ct_save.notes);
    if(_img)
    {
        _img->set_img(_ct_save.icon);
        _img->update_img();
    }

    _edit.account->setText(_ct_save.account);
    _edit.phone->setText(_ct_save.phone);
    _edit.age->setText(_ct_save.age);
    _edit.sex->setText(_ct_save.sex);
    _edit.nickname->setText(_ct_save.nickname);
    _edit.location->setText(_ct_save.location);
}

ct_ac_info wid_person_info::get_edit_info()
{
    int64 sex;
    if(_edit.sex->text() == "女") { sex = 0; }
    else if(_edit.sex->text() == "男") { sex = 1; }

    ct_ac_info ct;
    ct.account  = _edit.account->text().toLongLong();
    ct.phone    = _edit.phone->text().toLongLong();
    ct.age      = _edit.age->text().toLongLong();
    ct.sex      = sex;
    ct.nickname = qstos(_edit.nickname->text());
    ct.location = qstos(_edit.location->text());
    return ct;
}

string wid_person_info::get_edit_remarks()
{
    QString str;
    if(_edit.remarks) str = _edit.remarks->text();
    return str.toStdString();
}

string wid_person_info::get_edit_notes()
{
    QString str;
    if(_edit.notes) str = _edit.notes->get_send_text();
    return str.toStdString();
}
