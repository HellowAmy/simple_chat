#include "qarea_wid.h"

//#include "Tvlog.h"

qarea_wid::qarea_wid(QWidget *parent)
    : QScrollArea(parent)
{
    _style = R"(
        QScrollArea {
            border: none;
        }
        QScrollBar:vertical {
            background-color: #F5F5F5;
            width: %1px;
        }
        QScrollBar::handle:vertical {
           background-color: #C3C3C3;
           border-radius: 50%;
        }
        QScrollBar::add-line:vertical,
        QScrollBar::sub-line:vertical {
           background: none;
        }
)";

    this->setWidget(&_wid_area);
}

void qarea_wid::set_border(int width)
{
    _style = _style.arg(width);
    this->setStyleSheet(_style);
}

void qarea_wid::init_size(QSize size)
{
    set_border(2);
    _wid_area.resize(size.width() - 5,0);

    this->setWidget(&_wid_area);
    this->resize(size);
    this->show();
}

void qarea_wid::add_wid(QWidget *wid,int space)
{
    //检查不触底时不滚动
    bool is_bottom = check_bottom_bar();

    wid->setParent(&_wid_area);
    wid->show();
    wid->move(0,_wid_area.height());
    add_height_wid(wid->height() + space);

    if(is_bottom) show_bottom_bar();
}

void qarea_wid::add_width_wid(int width)
{
    _wid_area.resize(_wid_area.width() + width,_wid_area.height());
}

void qarea_wid::add_height_wid(int height)
{
    _wid_area.resize(_wid_area.width(),_wid_area.height() + height);
}

void qarea_wid::show_bottom_bar()
{
    QScrollBar* bar = this->verticalScrollBar();
    bar->setValue(bar->maximum());
}

bool qarea_wid::check_bottom_bar()
{
    QScrollBar* bar = this->verticalScrollBar();
    if(bar->value() == bar->maximum()) return true;
    return false;
}

QWidget *qarea_wid::get_area()
{
    return &_wid_area;
}
