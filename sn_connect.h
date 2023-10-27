#ifndef SN_CONNECT_H
#define SN_CONNECT_H

#include <QWidget>
#include <QObject>

#include "Tvlog.h"
#include "typedef_struct.h"
#include "qweb_files.h"

using namespace typedef_struct;

//!
//! 类说明： 显示图片，可保持比例显示与设置大小，必须调用 update_img 函数
//!
class sn_connect : public QObject
{
    Q_OBJECT
public:
    explicit sn_connect(QObject *parent = nullptr) : QObject(parent){}

    void download_icon(int64 account,std::function<void(bool ok,int64 account,string save_path)> cb)
    {
        qweb_ac_download *wcf = new qweb_ac_download(this);
        wcf->download_icon_ac(account,cb);
        wcf->open();
    }

signals:
    void sn_extend_add_find(int64 types,int64 account);
    void sn_extend_add_find_back(ct_ac_info info);



    void sn_extend_add_ask();

protected:

};

typedef Tsingle_d<sn_connect> sn_connect_t;

#endif // SN_CONNECT_H
