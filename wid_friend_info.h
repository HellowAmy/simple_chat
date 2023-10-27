#ifndef WID_FRIEND_INFO_H
#define WID_FRIEND_INFO_H

#include <QWidget>
#include <QVector>
#include <QLineEdit>
#include <QFileDialog>
#include <QImageReader>
//#include <QFileInfo>

#include "qframe_line.h"
#include "qbutt_line.h"
#include "qlab_img.h"
#include "qmove_pos.h"
#include "wid_person_info.h"
#include "wid_friend_add.h"

//!
//! 类说明： 聊天好友信息，需要调用update_info更新界面
//!
class wid_friend_info : public qframe_line
{
    Q_OBJECT
public:
    explicit wid_friend_info(QWidget *parent = nullptr);
    void init_info();               //更新界面
//    void update_info();             //更新界面
    void set_icon(QString icon);    //设置头像
    void set_name(QString name);    //设置昵称
    void show_extend();

    //设置扩展区父类与位置大小
    void set_extend_wid(QWidget *parent);

//    QWidget* get_extend_wid();  //获取扩展窗口，用于贴附到其他界面
    wid_person_info* get_person();

signals:
    void sn_show_extend();
    void sn_change_icon(QString file);
    void sn_click_name(QString name);   //点击名字按钮
    void sn_click_extend(QString tips); //点击扩展按钮
    void sn_info_all();

private:
    int _space;     //间隔
//    QString _icon;  //头像
//    QString _name;  //昵称
    qbutt_line *_butt_extend;       //扩展按钮
    qbutt_line *_butt_name;         //昵称按钮
    qframe_line *_wid_icon;         //头像显示
    qframe_line *_wid_extend;       //扩展显示
    qlab_img *_wid_img;             //头像图片
    wid_person_info *_wid_person;   //个人资料
    wid_friend_add *_wid_add;

    std::vector<QWidget*> _vec_extend;

    void status_extend(int status,QWidget* wid);
    void show_wid_extend(QWidget* wid);

    bool is_img(QString file);

//    QVector<QString> _vec_extend;   //扩展功能按钮组合
};

#endif // WID_FRIEND_INFO_H
