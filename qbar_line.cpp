#include "qbar_line.h"

qbar_line::qbar_line(QWidget *parent)
    : QProgressBar(parent)
{
    _style = R"(
        QProgressBar {
            background-color: lightgray;
            color: transparent;
            border: none;
        }
        QProgressBar::chunk {
            background-color: gray;
        }
)";

    this->resize(parent->width(),parent->height()/100);
    this->setStyleSheet(_style);
    this->show();
}
