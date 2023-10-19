#ifndef WID_DIALOG_BOX_H
#define WID_DIALOG_BOX_H

#include <QDialog>
#include <QMessageBox>

#include "typedef_struct.h"
#include "qframe_line.h"
#include "qbutt_line.h"
#include "qlab_wrap.h"
#include "qmove_pos.h"

using namespace typedef_struct;

//!
//! 类说明：提示消息盒
//!
class wid_dialog_box : public QDialog
{
public:
    enum en_level{ e_info,e_debug,e_warning,e_error };

public:
    explicit wid_dialog_box(QWidget *parent = nullptr) : QDialog(parent)
    {
        this->setModal(true);
        _lab = new qlab_wrap(this);

        _butt_no = new qbutt_line(this);
        _butt_no->set_text("取消");

        _butt_ok = new qbutt_line(this);
        _butt_ok->set_text("确认");

        parent->connect(_butt_no,&qbutt_line::sn_clicked,this,[=](){
            _ok = false;
            this->close();
        });
        parent->connect(_butt_ok,&qbutt_line::sn_clicked,this,[=](){
            _ok = true;
            this->close();
        });
    }

    bool make_box(QString title,QString txt,en_level en = e_info,int max_width = 300,int high = 100)
    {
        int space = 5;
        int butt_high = 30;

        auto [a,b] = fn_Label(this,txt,{max_width,high});
        if(en == e_debug)
        {
            qframe_line *f = a;
            f->set_col(QColor(0x00aa00));
        }
        else if(en == e_warning)
        {
            qframe_line *f = a;
            f->set_col(QColor(0xaaaa00));
        }
        else if(en == e_error)
        {
            qframe_line *f = a;
            f->set_col(QColor(0xaa0000));
        }

        _butt_no->resize(max_width/2 - space,butt_high);
        _butt_ok->resize(max_width/2 - space,butt_high);

        qmove_pos move;
        move.move_center(this);
        {
            move.add_wid(_butt_no);
            move.add_wid(_butt_ok);
        }
        move.move_x({0,high + 5},space*2);

        this->setWindowTitle(title);
        this->setFixedSize(max_width,high + butt_high + space *2);
        this->exec();
        return _ok;
    }

protected:
    bool _ok = false;
    qlab_wrap *_lab;
    qbutt_line *_butt_no;
    qbutt_line *_butt_ok;
};

#endif // WID_DIALOG_BOX_H
