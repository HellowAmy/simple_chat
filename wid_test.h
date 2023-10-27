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

    void wid_frame(QWidget *wid);

    void test_0(QWidget *parent);
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
    void test_15(QWidget *parent);
    void test_16(QWidget *parent);
    void test_17(QWidget *parent);
    void test_18(QWidget *parent);
    void test_19(QWidget *parent);
    void test_20(QWidget *parent);
    void test_21(QWidget *parent);
    void test_22(QWidget *parent);
    void test_23(QWidget *parent);
    void test_24(QWidget *parent);
    void test_25(QWidget *parent);
    void test_26(QWidget *parent);
    void test_27(QWidget *parent);
    void test_28(QWidget *parent);
    void test_30(QWidget *parent);
    void test_31(QWidget *parent);
    void test_32(QWidget *parent);

protected:

    void closeEvent(QCloseEvent *event) override
    {
        event->ignore();
        qDebug()<<"closeEvent ww";
        QWidget::closeEvent(event);
    }

};
#endif // WID_TEST_H
