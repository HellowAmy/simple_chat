#ifndef QEDIT_TXT_H
#define QEDIT_TXT_H

#include <QTextEdit>
#include <QKeyEvent>
#include <QDropEvent>
#include <QMimeData>

//!
//! 类说明： 输入框架重写，可回车与拖入图片
//!
class qedit_txt : public QTextEdit
{
    Q_OBJECT
public:
    explicit qedit_txt(QWidget *parent = nullptr);

    void set_border(int width = 2);     //滑动条宽度设置
    QString get_send_text();            //获取发送的文字

signals:
    void sn_send_enter();               //按下回车按键
    void sn_send_drop(QList<QUrl> ls);  //拖动图片

protected:
    void keyPressEvent(QKeyEvent *event) override;  //回车按键重写
    void dropEvent(QDropEvent* event) override;     //拖动图片重写

private:
    QString _style; //滑动区样式表
};

#endif // QEDIT_TXT_H
