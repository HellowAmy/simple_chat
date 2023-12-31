#ifndef WID_PERSON_INFO_H
#define WID_PERSON_INFO_H

#include <QWidget>
#include <QIntValidator>
#include <QLineEdit>

#include "wid_dialog_box.h"
#include "qbutt_line.h"
#include "typedef_struct.h"
#include "qframe_line.h"
#include "qmove_pos.h"

using namespace typedef_struct;

class wid_person_info : public qframe_line
{
    Q_OBJECT
public:
    struct ct_person_str
    {
        QString notes;
        QString icon;
        QString remarks;
        QString account;
        QString phone;
        QString age;
        QString sex;
        QString nickname;
        QString location;
    };
    struct ct_person_edit
    {
        QLineEdit *remarks  = nullptr;
        QLineEdit *account  = nullptr;
        QLineEdit *phone    = nullptr;
        QLineEdit *age      = nullptr;
        QLineEdit *sex      = nullptr;
        QLineEdit *nickname = nullptr;
        QLineEdit *location = nullptr;
        qedit_txt *notes    = nullptr;
    };

public:
    explicit wid_person_info(QWidget *parent = nullptr);
    void set_info(const ct_ac_info &info);
    void set_info_remarks(QString remarks = "",QString icon = "",QString notes = "");
    void init_edit();
    void init_friends();
    void init_add_ask();
    void init_add_recv();

    bool not_change_info();
    bool not_change_remarks();
    bool is_verification_info();
    void update_info();

    ct_ac_info get_edit_info();
    string get_edit_remarks();
    string get_edit_notes();

signals:
    void sn_save(bool save);
    void sn_save_info(ct_ac_info ct);
    void sn_save_remarks(string remarks);
    void sn_add_friend(ct_ac_info ct,string remarks,string notes);
    void sn_not_change();

protected:
    int _space;
    QSize _size_lab;
    QSize _size_edit;
    ct_person_edit _edit;
    ct_person_str _ct_save;
    qbutt_line *_cancel;
    qbutt_line *_save;
    qbutt_line *_add;
    qlab_img *_img;
};

#endif // WID_PERSON_INFO_H
