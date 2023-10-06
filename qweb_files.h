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

class qweb_files : public QObject
{
    Q_OBJECT
public:
    struct fs_status
    {
        bool stop;          //是否停止
        string filename;    //文件名称
    };

public:
    explicit qweb_files(QObject *parent = nullptr);

    int open(string ip = CS_SERVER_ADDRESS,int port = CS_PORT_FILES,string txt = protocol::_head_);

    //!
    //! 上传文件：
    //!     1.发送上传创建
    //!     2.获取创建反馈，发送文件数据，发送完成提交结果
    //!     3.获取结果反馈，完成文件上传
    //!
    bool upload_file(int64 time, const string &path);


    //!
    //! 下载文件：
    //!     1.下载文件创建
    //!     2.获取文件信息反馈，建立文件写入流
    //!     3.发送准备完成文件ID，等待服务器下发数据分段
    //!     4.获取服务器下发完成反馈，结束文件下载
    //!
    bool download_file(int64 id);

    //!     uint time           时间序号
    //!     uint target         目标账号
    //!     uint source         源址账号
    //!     uint length_max     文件长度
    //!     string filename     文件名称

signals:
    void sn_open();
    void sn_close();
//    void sn_file_recv(int64 id);

protected:
    string _path_temp_save; //保存路径
    inter_client _wc;       //网络链接
    files_channel _fs_swap; //文件传输

//    std::queue<int64> _que_recv_ask;
    std::map<string,function<void(const string&)>> _map_fn;     //任务函数索引
    std::map<int64,fs_status> _map_upload_status;               //发送文件名索引

    int64 get_id_channel();

    void sl_open();
    void sl_message(const string &msg);
    void sl_close();

    bool send_msg(const string &json);
    bool send_data(int64 id,const string &msg);
    bool check_high_line();

    void task_recv_binary_data(int64 id,const string &data);

    void task_files_create_upload_back(const string &sjson);
    void task_files_finish_upload_back(const string &sjson);

    void task_files_create_download_back(const string &sjson);
    void task_files_finish_download(const string &sjson);

    void task_files_cancel_download_back(const string &sjson);

};

#endif // QWEB_FILES_H
