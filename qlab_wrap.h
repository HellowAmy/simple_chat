#ifndef QLAB_WRAP_H
#define QLAB_WRAP_H

#include <QWidget>
#include <QPainter>
#include <QFontMetrics>

//!
//! 类说明： 显示文字标签，可自动换行适应文字大小，必须调用 update_text 函数
//!
class qlab_wrap : public QWidget
{
    Q_OBJECT
public:
    explicit qlab_wrap(QWidget *parent = nullptr);

    void set_text(QString text);    //设置文字
    void set_font(QFont font);      //文字样式
    void set_width_max(int width);  //设置文字
    void set_frame(bool frame);     //显示边框
    QString get_text();             //返回文字
    void update_text();             //更新文字显示

protected:
    //绘图事件
    void paintEvent(QPaintEvent *event) override;

private:
    bool _frame;        //显示线框
    int _max_width;     //最大宽度
    QColor _col;        //线条颜色
    QFont _font;        //文字样式
    QString _text;      //文字保存
    QString _text_new;  //显示文字年

    //计算换行符
    int calc_break(const QString &txt);

    //计算带换行最大字符宽度
    int calc_max_width(const QString &txt,const QFontMetrics &fm);

    //超过最大宽度时插入换行符 -- 通过计算字符长度,返回插入换行符的新字符
    QString insert_break(const QString &txt,const QFontMetrics &fm,int width);
};

#endif // QLAB_WRAP_H
