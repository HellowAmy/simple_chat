#ifndef WID_FRIEND_ADD_H
#define WID_FRIEND_ADD_H

#include <QWidget>

#include "sn_connect.h"
#include "wid_person_info.h"

class wid_friend_add : public qframe_line
{
    Q_OBJECT
public:
    explicit wid_friend_add(QWidget *parent = nullptr);

    wid_person_info* get_person();

signals:

private:
    qbutt_line *_butt;
    QLineEdit *_edit;
    wid_person_info *_info;
};

#endif // WID_FRIEND_ADD_H
