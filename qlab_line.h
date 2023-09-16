#ifndef QLAB_LINE_H
#define QLAB_LINE_H

#include <QWidget>
#include <QPainter>

class qlab_line : public QWidget
{
    Q_OBJECT
public:
    explicit qlab_line(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QColor _col;    //线条颜色
};

#endif // QLAB_LINE_H
