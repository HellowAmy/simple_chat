#include "wid_test.h"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("Fusion"));

    wid_test w;
//    w.show();

    return a.exec();
}


