#ifndef QFRAME_LINE_H
#define QFRAME_LINE_H

#include <QWidget>
#include <QPainter>

//!
//! 类说明： 显示消息背景线条框
//!
class qframe_line : public QWidget
{
    Q_OBJECT
public:
    explicit qframe_line(QWidget *parent = nullptr);

    void set_frame(bool frame); //设置线框
    void set_col(QColor col);   //设置颜色
    void move_center(QWidget *wid,int padding = 5);             //居中子对象--不改变大小
    void resize_center(QWidget *wid,QSize size, int padding);   //居中子对象--改变大小
    QSize calc_size(QSize max,int padding);     //计算减去边距的大小

protected:
    //绘图事件
    void paintEvent(QPaintEvent *event) override;

private:
    bool _frame;    //线框
    QColor _col;    //线条颜色
};

#endif // QFRAME_LINE_H
