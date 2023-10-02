#ifndef QWEB_FILES_H
#define QWEB_FILES_H

#include <QObject>
#include <QQueue>
#include <QMap>
#include <QIterator>
#include <QTimer>
#include <functional>
#include <thread>
#include <condition_variable>

#include "include/hv/WebSocketClient.h"
#include "web_protocol.h"
#include "inter_client.h"
#include "swap_files.h"

using namespace protocol;
using std::function;
using std::string;
using hv::WebSocketClient;

class qweb_files : public QObject
{
    Q_OBJECT
public:
    enum en_status { e_first,e_pause,e_sending,e_end };
    struct fs_status
    {
        en_status state;    //传输状态
        bool stop;          //是否停止
        bool swap;          //是否交换
        int64 swap_id;      //临时文件
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
    bool upload_file(int64 time, int64 source, const string &path);

    //!     uint time           时间序号
    //!     uint target         目标账号
    //!     uint source         源址账号
    //!     uint length_max     文件长度
    //!     string filename     文件名称

signals:
    void sn_open();
    void sn_close();

protected:
    inter_client _wc;       //网络链接
    swap_files _swap_fs;    //文件传输

//    QTimer t;

    std::map<string,function<void(const string&)>> _map_fn;     //任务函数索引
    std::map<int64,fs_status> _map_upload_status;               //发送文件名索引

    void sl_open();
    void sl_message(const string &msg);
    void sl_close();

    bool send_msg(const string &json);
    bool send_data(int64 id,const string &msg);
    bool check_high_line();


    void task_files_create_upload_back(const string &sjson);
    void task_files_finish_upload_back(const string &sjson);
    void task_files_create_download_back(const string &sjson);
    void task_files_finish_download(const string &sjson);
    void task_files_cancel_download_back(const string &sjson);

};

#endif // QWEB_FILES_H
