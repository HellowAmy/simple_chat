#ifndef QBAR_LINE_H
#define QBAR_LINE_H

#include <QProgressBar>

//!
//! 类说明： 进度条样式，显示线条状且无文字提示
//!
class qbar_line : public QProgressBar
{
    Q_OBJECT
public:
    explicit qbar_line(QWidget *parent = nullptr);

private:
    QString _style; //样式保存
};

#endif // QBAR_LINE_H
