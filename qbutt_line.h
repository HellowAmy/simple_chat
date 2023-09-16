#ifndef QBUTT_LINE_H
#define QBUTT_LINE_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

//!
//! 类说明： 重写按钮类，三状态显示黑白线条样式
//!
class qbutt_line : public QWidget
{
    Q_OBJECT

public:
    enum status
    {
        e_leave,
        e_enter,
        e_press,
    };

public:
    explicit qbutt_line(QWidget *parent = nullptr);

    void keep_status(bool keep,status st = e_enter);
    void set_col(QColor col);   //设置颜色
    void set_text(QString txt); //设置文字
    void set_font(QFont font);  //设置文字样式
    QString get_text();         //获取文字
    QFont get_font();           //获取文字样式
    QColor get_col();           //获取文字样式

signals:
    void sn_clicked();  //点击信号

protected:
    //绘图事件
    void paintEvent(QPaintEvent *event) override;

    //触发事件
    void enterEvent(QEnterEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    bool _keep;
    status _kep_st; //三状态标记
    status _status; //三状态标记
    QColor _col;    //线条颜色
    QString _text;  //文字
    QFont _font;    //文字样式

    void switch_status(status st);  //控制三状态切换
};

#endif // QBUTT_LINE_H
