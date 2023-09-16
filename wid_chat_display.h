#ifndef WID_CHAT_DISPLAY_H
#define WID_CHAT_DISPLAY_H

#include <QWidget>

#include "wid_message.h"

class wid_chat_display : public QWidget
{
    Q_OBJECT
public:
    explicit wid_chat_display(QWidget *parent = nullptr);

signals:

private:
//    int
//    QSize _size_frame;

//    QLabel _lab_head;
//    QLabel _lab_head;
//    wid_message *_message;

};

#endif // WID_CHAT_DISPLAY_H
