#ifndef WID_CHAT_LIST_H
#define WID_CHAT_LIST_H

#include "qframe_line.h"
#include "wid_chat.h"

#include <QVector>

//!
//! 类说明： 聊天好友列表，显示按钮和聊天框
//!
class wid_chat_list : public qframe_line
{
public:
    struct ct_friend
    {
        QString name;
        QString icon;
        wid_chat *chat;
    };

    Q_OBJECT
public:
    explicit wid_chat_list(QWidget *parent = nullptr);
    void add_friends_vec(const QVector<QString> &vec);

signals:

private:
    QVector<QString> _vec_friends;

};

#endif // WID_CHAT_LIST_H
