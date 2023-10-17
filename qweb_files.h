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
class qweb_download_icon : public qweb_files
{
    Q_OBJECT
public:
    struct ct_data
    {
        int64 id;
        int64 account;
        string abs_path;
    };

signals:
    void sn_download_icon(int64 account,string path);
    void sn_upload_icon(int64 account,string path);

public:
    explicit qweb_download_icon(QObject *parent = nullptr)
    {

        connect(this,&qweb_download_icon::sn_create_download,[=](bool ok,int64 id,const string &abs_path){
            if(ok)
            {
                for(auto &a:_vec_download)
                {
                    if(a.abs_path == abs_path)
                    {
                        a.id = id;
                        break;
                    }
                }
            }
        });

        connect(this,&qweb_download_icon::sn_finish_download,[=](bool ok,int64 id){
            for(auto it = _vec_download.begin();it != _vec_download.end();it++)
            {
                if(it->id == id)
                {
                    if(ok) emit sn_download_icon(it->account,it->abs_path);
                    _vec_download.erase(it);
                    break;
                }
            }
        });

        connect(this,&qweb_download_icon::sn_create_upload,[=](bool ok,int64 id,const string &abs_path){
            if(ok)
            {
                for(auto &a:_vec_upload)
                {
                    if(a.abs_path == abs_path)
                    {
                        a.id = id;
                        break;
                    }
                }
            }
        });

        connect(this,&qweb_download_icon::sn_finish_upload,[=](bool ok,int64 id){
            for(auto it = _vec_upload.begin();it != _vec_upload.end();it++)
            {
                if(it->id == id)
                {
                    if(ok) emit sn_upload_icon(it->account,it->abs_path);
                    _vec_upload.erase(it);
                    break;
                }
            }
        });
    }

    bool download_icon_ac(int64 account)
    {
        string path;
        bool ok = download_icon(account,path);
        if(ok)
        {
            ct_data ct{0,account,path};
            _vec_download.push_back(ct);
        }
        return ok;
    }

    bool upload_icon_ac(const string &path,int64 account)
    {
        bool ok = upload_icon(path,account);
        if(ok)
        {
            ct_data ct{0,account,path};
            _vec_upload.push_back(ct);
        }
        return ok;
    }

protected:
    vector<ct_data> _vec_download;
    vector<ct_data> _vec_upload;
};

#endif // QWEB_FILES_H
