#ifndef QLAB_BUBBLE_H
#define QLAB_BUBBLE_H

#include <QWidget>
#include <QPainter>

//!
//! 类说明： 显示消息的气泡背景
//!
class qlab_bubble : public QWidget
{
    Q_OBJECT
public:
    explicit qlab_bubble(QWidget *parent = nullptr);

    void set_col_backdrop(QColor col); //设置背景颜色

protected:
    //绘图事件
    void paintEvent(QPaintEvent *event) override;

private:
    int _radius;            //圆角弧度
    QColor _col_backdrop;   //背景颜色
};

#endif // QLAB_BUBBLE_H
