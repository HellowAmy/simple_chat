#ifndef QWEB_FILES_H
#define QWEB_FILES_H

#include <QObject>
#include <QQueue>
#include <QMap>
#include <QIterator>
#include <QTimer>
#include <functional>
#include <thread>
#include <queue>
#include <condition_variable>

#include "include/hv/WebSocketClient.h"
#include "web_protocol.h"
#include "inter_client.h"
//#include "swap_files.h"
//#include "Tvlog.h"
#include "files_transfer.h"

using namespace protocol;
using std::function;
using std::string;
using hv::WebSocketClient;

//!
//! 类说明： 文件传输，与服务器对接文件传输与下载功能
//!
class qweb_files : public QObject
{
    Q_OBJECT
public:
    explicit qweb_files(QObject *parent = nullptr);

    int open(string ip = CS_SERVER_ADDRESS,int port = CS_PORT_FILES,string txt = protocol::_head_);
    void close();

    //!
    //! 上传文件：
    //!     1.发送上传创建
    //!     2.获取创建反馈，发送文件数据，发送完成提交结果
    //!     3.获取结果反馈，完成文件上传
    //!
    //! abs_path: 上传绝对路径，save_path：服务器保存路径，为空是使用临时路径
    //!
    bool upload_file(const string &abs_path,const string &save_path = "");
    bool upload_icon(const string &abs_path,int64 account);

    //!
    //! 下载文件：
    //!     1.下载文件创建
    //!     2.获取文件信息反馈，建立文件写入流
    //!     3.发送准备完成文件ID，等待服务器下发数据分段
    //!     4.获取服务器下发完成反馈，结束文件下载
    //!
    //! abs_path: 服务器下载的绝对路径，save_path：客户端保存路径，为空是使用临时路径
    //!
    bool download_file(const string &abs_path,const string &save_path = "");
    bool download_icon(int64 account,string &save_path);

    string get_temp_path();
    string get_icon_path();
    inter_client* get_wc();

signals:
    void sn_open();
    void sn_close();

    void sn_create_upload(bool ok,int64 id,const string &abs_path);
    void sn_create_download(bool ok,int64 id,const string &abs_path);

    void sn_finish_upload(bool ok,int64 id);
    void sn_finish_download(bool ok,int64 id);

    void sn_prog_upload(int64 id,int64 prog);
    void sn_prog_download(int64 id,int64 prog);

protected:
    string _path_temp;      //保存路径
    string _path_icon;      //头像路径
    inter_client _wc;       //网络链接
    files_channel _fs_swap; //文件传输

    std::map<string,function<void(const string&)>> _map_fn;     //任务函数索引

    int64 get_id_channel();

    void sl_open();
    void sl_message(const string &msg);
    void sl_close();

    bool send_msg(const string &json);
    bool send_data(int64 id,const string &msg);

    void task_recv_binary_data(int64 id,const string &data);

    void task_files_create_upload_back(const string &sjson);
    void task_files_begin_upload_back(const string &sjson);
    void task_files_finish_upload_back(const string &sjson);

    void task_files_create_download_back(const string &sjson);
    void task_files_begin_download_back(const string &sjson);
    void task_files_finish_download_back(const string &sjson);
};


//!
//! 类说明：下载头像
//!
class qweb_ac_download : public qweb_files
{
public:
    int64 _account;
    string _save_path;
    std::function<void(bool ok,int64 account,string save_path)> _fn_download_cb = nullptr;

    explicit qweb_ac_download(QObject *parent = nullptr)
    {
        connect(this,&qweb_ac_download::sn_open,[=](){
            bool ok = download_icon(_account,_save_path);
            if(ok == false) { if(_fn_download_cb) _fn_download_cb(ok,_account,_save_path); }
        });

        connect(this,&qweb_ac_download::sn_finish_download,[=](bool ok,int64 id){
            if(_fn_download_cb) _fn_download_cb(ok,_account,_save_path);
            this->close();
        });
    }

    void download_icon_ac(int64 account,
            std::function<void(bool ok,int64 account,string save_path)> fn_download_cb)
    {
        _account = account;
        _fn_download_cb = fn_download_cb;
    }
};

//!
//! 类说明：上传头像
//!
class qweb_ac_upload : public qweb_files
{
public:
    int64 _account;
    string _abs_path;
    std::function<void(bool ok,int64 account,string save_path)> _fn_upload_cb = nullptr;

    explicit qweb_ac_upload(QObject *parent = nullptr)
    {
        connect(this,&qweb_ac_upload::sn_open,[=](){
            bool ok = upload_icon(_abs_path,_account);
            if(ok == false) { if(_fn_upload_cb) _fn_upload_cb(ok,_account,_abs_path); }
        });

        connect(this,&qweb_ac_upload::sn_finish_upload,[=](bool ok,int64 id){
            if(_fn_upload_cb) _fn_upload_cb(ok,_account,_abs_path);
            this->close();
        });
    }

    void upload_icon_ac(int64 account,string abs_path,
                          std::function<void(bool ok,int64 account,string save_path)> fn_upload_cb)
    {
        _account = account;
        _abs_path = abs_path;
        _fn_upload_cb = fn_upload_cb;
    }
};


#endif // QWEB_FILES_H
