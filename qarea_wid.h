#ifndef QAREA_WID_H
#define QAREA_WID_H

#include <QWidget>
#include <QScrollArea>
#include <QScrollBar>

//!
//! 类说明： 滑动区域，滑动与显示区域组合，超范围会显示进度条
//!
class qarea_wid : public QScrollArea
{
    Q_OBJECT
public:
    explicit qarea_wid(QWidget *parent = nullptr);

    void set_border(int width = 2);         //滑动条宽度
    void init_size(QSize size);             //初始化显示区域和滑动区域大小
    void add_wid(QWidget *wid,int space);   //加入到显示区
    void add_width_wid(int width);          //增加显示区域--宽度
    void add_height_wid(int height);        //增加显示区域--高度
    void show_bottom_bar();                 //滚到底部
    bool check_bottom_bar();                //检查是否为底部
    QWidget* get_area();                    //获取显示区域

private:
    QString _style;     //滑动区样式表
    QWidget _wid_area;  //显示区域
};

#endif // QAREA_WID_H
