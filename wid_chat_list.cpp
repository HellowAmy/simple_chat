#include "wid_chat_list.h"

wid_chat_list::wid_chat_list(QWidget *parent)
    : qframe_line(parent)
{

}

void wid_chat_list::add_friends_vec(const QVector<QString> &vec)
{
    _vec_friends = vec;
}
