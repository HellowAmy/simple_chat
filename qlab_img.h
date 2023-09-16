#ifndef QLAB_IMG_H
#define QLAB_IMG_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>

//!
//! 类说明： 显示图片，可保持比例显示与设置大小，必须调用 update_img 函数
//!
class qlab_img : public QWidget
{
    Q_OBJECT
public:
    explicit qlab_img(QWidget *parent = nullptr);
    void set_keep(bool keep,QSize size = QSize(0,0));   //保持比例,keep时size只采取width，否则按照size大小显示
    void set_frame(bool frame);     //显示边框
    void set_img(QString file);     //设置图片
    void update_img();              //更新显示
    QString get_file();             //获取文件

signals:

protected:
    //绘图事件
    void paintEvent(QPaintEvent *event) override;

private:
    bool _keep;     //保持范围
    bool _frame;    //显示范围框
    QSize _size;    //图片大小
    QString _file;  //文件名
    QPixmap _pix;   //绘制图像
};

#endif // QLAB_IMG_H
