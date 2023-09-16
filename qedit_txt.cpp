#include "qedit_txt.h"

qedit_txt::qedit_txt(QWidget *parent)
    : QTextEdit(parent)
{
    _style = R"(
        QTextEdit {
            font-family: 微软雅黑;
            border: none;
            font-size: 16px;
            color: #808080;
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

    set_border(2);
}

void qedit_txt::set_border(int width)
{
    _style = _style.arg(width);
    this->setStyleSheet(_style);
}

QString qedit_txt::get_send_text()
{
    QString txt = this->toPlainText();
    this->clear();
    return txt;
}

void qedit_txt::keyPressEvent(QKeyEvent *event)
{
    if(event->modifiers() == Qt::NoModifier
        && (event->key() == Qt::Key_Return
            || event->key() == Qt::Key_Enter))
    { emit sn_send_enter(); }
    else QTextEdit::keyPressEvent(event);
}

void qedit_txt::dropEvent(QDropEvent *event)
{
    const QMimeData* mimeData = event->mimeData();
    if (mimeData->hasUrls()) emit sn_send_drop(mimeData->urls());
}
