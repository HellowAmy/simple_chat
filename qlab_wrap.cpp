#include "qlab_wrap.h"
#include <QStyle>
qlab_wrap::qlab_wrap(QWidget *parent)
    : QWidget(parent)
{
    this->show();
    _frame = false;
    _col = Qt::darkGray;
    _max_width = 500;
    _font = QFont("微软雅黑",12);
}

void qlab_wrap::set_text(QString text)
{
    _text = text;
}

void qlab_wrap::set_font(QFont font)
{
    _font = font;
}

void qlab_wrap::set_width_max(int width)
{
    _max_width = width;
}

void qlab_wrap::set_frame(bool frame)
{
    _frame = frame;
}

QString qlab_wrap::get_text()
{
    return _text;
}

void qlab_wrap::paintEvent(QPaintEvent *event)
{
    QPainter show(this);
    show.setFont(_font);
    show.setPen(QPen(_col,2));
    if(_frame) show.drawRect(this->rect());
    show.drawText(this->rect(),Qt::AlignLeft | Qt::AlignVCenter,_text_new);
    show.end();
}

int qlab_wrap::calc_break(const QString &txt)
{
    int ret = 0;
    for(int i=0;i<txt.size();i++)
    {
        if(txt[i] == '\n') { ret ++; }
    }
    return ret;
}

int qlab_wrap::calc_max_width(const QString &txt,const QFontMetrics &fm)
{
    int max = 0;
    QStringList ls = txt.split("\n");
    for(int i=0;i<ls.size();i++)
    {
        int width = fm.horizontalAdvance(ls[i]);
        if(max < width) max = width;
    }
    return max;
}

QString qlab_wrap::insert_break(const QString &txt, const QFontMetrics &fm, int width)
{
    QString new_txt;
    int count = 0;
    for(int i=0;i<txt.size();i++)
    {
        int chat_width = fm.horizontalAdvance(txt[i]);
        count += chat_width;
        if(count >= width)
        {
            count = chat_width;
            new_txt += "\n";
        }
        new_txt += txt[i];
    }
    return new_txt;
}

void qlab_wrap::update_text()
{
    //!
    //! 1.超过最宽度插入换行符
    //! 2.计算换行符数量
    //! 3.设置显示的文字范围
    //!
    QFontMetrics fm(_font);
    _text_new = insert_break(_text,fm,_max_width);
    int line_wrap = calc_break(_text_new) +1;
    int line_max = calc_max_width(_text,fm);
    QRect rect_text(0,0,(line_max > _max_width ? _max_width : line_max),fm.height() * line_wrap);
    this->resize(rect_text.size());
}
