#ifndef WID_MSG_ABS_H
#define WID_MSG_ABS_H

#include <QWidget>

#include "qframe_line.h"
#include "qlab_wrap.h"
#include "qlab_img.h"

//!
//! 类说明： 显示消息基类，保存显示信息的内容与类型
//!
class wid_msg_abs : public qframe_line
{
    Q_OBJECT
public:
    explicit wid_msg_abs(QWidget *parent = nullptr);
    virtual void update_msg(int max_width) = 0; //更新大小
    QString get_type();     //显示类型
    QString get_content();  //保存内容
    QWidget* get_area();    //显示区域

protected:
    int _padding;       //内容边距
    QString _type;      //显示类型
    QString _content;   //保存内容
    QWidget *_wid_area; //显示区域
};

//!
//! 类说明： 显示消息--文字
//!
class wid_msg_text : public wid_msg_abs
{
    Q_OBJECT
public:
    explicit wid_msg_text(QString txt,QWidget *parent = nullptr);
    void update_msg(int max_width) override;
    qlab_wrap *_lab;
};

//!
//! 类说明： 显示消息--图片
//!
class wid_msg_img : public wid_msg_abs
{
    Q_OBJECT
public:
    explicit wid_msg_img(QString file,QWidget *parent = nullptr);
    void update_msg(int max_width) override;
    qlab_img *_lab;
};

//!
//! 类说明： 显示消息--提示
//!
class wid_msg_hint : public wid_msg_abs
{
    Q_OBJECT
public:
    explicit wid_msg_hint(QString hint,QWidget *parent = nullptr);
    void update_msg(int max_width) override;
    qlab_wrap *_lab;
};

#endif // WID_MSG_ABS_H
