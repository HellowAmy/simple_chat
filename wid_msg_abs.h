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
    virtual void update_msg(QString content,int max_width,int padding) = 0;

protected:
    int _padding = 5;       //内容边距
};

//!
//! 类说明： 显示消息--文字
//!
class wid_msg_text : public wid_msg_abs
{
    Q_OBJECT
public:
    void update_msg(QString content,int max_width,int padding) override
    {
        qlab_wrap *_lab = new qlab_wrap(this);
        _lab->set_text(content);

        int size_wid = max_width - padding*2;
        _lab->set_width_max(size_wid);
        _lab->update_text();
        this->move_center(_lab,padding);
    }
};

//!
//! 类说明： 显示消息--图片
//!
class wid_msg_img : public wid_msg_abs
{
    Q_OBJECT
public:
    void update_msg(QString content,int max_width,int padding) override
    {
        qlab_img *_lab = new qlab_img(this);
        _lab->set_img(content);

        QPixmap pix(_lab->get_file());
        int width = pix.width() > max_width ? max_width : pix.width();
        width -= padding*2;

        _lab->set_keep(true,QSize(width,width));
        _lab->update_img();
        this->move_center(_lab,padding);
    }
};

//!
//! 类说明： 显示消息--提示
//!
class wid_msg_hint : public wid_msg_abs
{
    Q_OBJECT
public:
    void update_msg(QString content,int max_width,int padding) override
    {
        this->set_frame(false);
        qlab_wrap *_lab = new qlab_wrap(this);
        _lab->set_text(content);
        _lab->set_font(QFont("微软雅黑",10));

        int size_wid = max_width - padding*2;
        _lab->set_width_max(size_wid);
        _lab->update_text();
        this->move_center(_lab,0);
    }
};

#endif // WID_MSG_ABS_H
