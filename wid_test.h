#ifndef WID_TEST_H
#define WID_TEST_H

#include <QWidget>
#include <QDebug>
#include <QCloseEvent>

//!
//! 类说明： 窗口测试类，所有的窗口控件的测试代码
//!
class wid_test : public QWidget
{
    Q_OBJECT

public:
    wid_test(QWidget *parent = nullptr);
    ~wid_test();

    void test_1(QWidget *parent);
    void test_2(QWidget *parent);
    void test_3(QWidget *parent);
    void test_4(QWidget *parent);
    void test_5(QWidget *parent);
    void test_6(QWidget *parent);
    void test_7(QWidget *parent);
    void test_8(QWidget *parent);
    void test_9(QWidget *parent);
    void test_10(QWidget *parent);
    void test_11(QWidget *parent);
    void test_12(QWidget *parent);
    void test_13(QWidget *parent);
    void test_14(QWidget *parent);

protected:

    void closeEvent(QCloseEvent *event) override
    {
        event->ignore();
        qDebug()<<"closeEvent ww";
        QWidget::closeEvent(event);
    }

};
#endif // WID_TEST_H
