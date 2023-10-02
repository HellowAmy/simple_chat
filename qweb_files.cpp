#include "qweb_files.h"

#include "Tvlog.h"

#define ADD_MAP(func) _map_fn.emplace(func,std::bind(&qweb_files::task_##func,this,std::placeholders::_1))

#define WRITE_BUFSIZE_HIGH_WATER    (1U << 23)  // 8M

qweb_files::qweb_files(QObject *parent)
    : QObject(parent)
{
    _wc.func_bind(std::bind(&qweb_files::sl_open,this),
                  std::bind(&qweb_files::sl_message,this,std::placeholders::_1),
                  std::bind(&qweb_files::sl_close,this));

    ADD_MAP(files_create_upload_back);
    ADD_MAP(files_finish_upload_back);
    ADD_MAP(files_create_download_back);
    ADD_MAP(files_finish_download);
    ADD_MAP(files_cancel_download_back);


    typedef std::shared_ptr<hv::WebSocketChannel> TSocketChannelPtr;
    _wc.get_wc()->onWriteComplete = [=](const TSocketChannelPtr& channel, hv::Buffer*) {

        //发送提示：利用ping/pong出发write发送回调，控制发送文件流量
        if(channel->isWriteComplete()
            && _map_upload_status.size() > 0)
        {
            for(auto it = _map_upload_status.begin();it != _map_upload_status.end();it++)
            {
                fs_status *st = &it->second;
                int64 id = st->swap_id;

                if(st->state == e_pause || st->state == e_first)
                {
                    //写入文件
                    bool is_end = false;
                    st->state = e_sending;
                    bool ok = _swap_fs.add_data_send(id,is_end);
                    st->state = e_pause;

                    //关闭文件
                    if(is_end || ok == false)
                    {
                        st->state = e_end;
                        _swap_fs.close_file_send(id);
                        _map_upload_status.erase(it);

                        //完成上传
                        string s = set_files_finish_upload(id,it->second.swap,is_end);
                        send_msg(s);

                        vlogd("finish swap file:" $(is_end));
                    }
                }
            }
        }
    };
}

int qweb_files::open(string ip, int port, string txt)
{
    return _wc.open(ip,port,txt);
}

bool qweb_files::upload_file(int64 time, int64 source, const string &path)
{
    if(swap_files::is_exists(path) == false
        || swap_files::is_can_read(path) == false
        || swap_files::is_file(path) == false) return false;

    int64 file_size = swap_files::get_file_size(path);
    string filename = swap_files::get_filename(path);
    string s = set_files_create_upload(time,source,source,file_size,filename);
    bool ok = send_msg(s);

    fs_status st{e_first,true,false,0,path};
    if(ok) _map_upload_status.emplace(time,st);
    return ok;
}

bool qweb_files::send_msg(const string &sjson)
{
    string s = set_files_json(sjson);
    _wc.get_wc()->send(s);
    return _wc.get_wc()->isConnected();
}

bool qweb_files::send_data(int64 id, const string &msg)
{
    string s = set_files_binary(id,msg);
    _wc.get_wc()->send(s);
    return _wc.get_wc()->isConnected();
}

bool qweb_files::check_high_line()
{
    size_t size = _wc.get_wc()->channel->writeBufsize();
    if(size >= WRITE_BUFSIZE_HIGH_WATER) return true;
    return false;
}

void qweb_files::task_files_create_upload_back(const string &sjson)
{
    //发送文件数据函数
    auto func_send = [=](int64 id,const string &data){
        send_data(id,data);
        return (check_high_line() == false);
    };

    //解析反馈
    int64 time;
    int64 id;
    bool ok;
    if(get_files_create_upload_back(sjson,time,id,ok))
    {
        if(ok)
        {
            //查找id定位文件
            auto it = _map_upload_status.find(time);
            if(it != _map_upload_status.end())
            {
                //打开文件
                fs_status *st = &it->second;
                st->swap_id = id;

                bool ok = _swap_fs.open_file_send(id,st->filename,func_send);
                vlogi("open:" $(id) $(ok));

                if(ok)
                {
                    //启动异步发送
                    hv::Buffer buf;
                    _wc.get_wc()->onWriteComplete(_wc.get_wc()->channel,&buf);
                }
            }
        }
    }
    vlogi("task_files_create_upload_back: " $(time) $(id) $(ok));
}

void qweb_files::task_files_finish_upload_back(const string &sjson)
{
    vlogi("task_files_create_upload_back");

    int64 swap_name;
    bool ok;
    if(get_files_finish_upload_back(sjson,swap_name,ok))
    {
        vlogd("get_files_finish_upload_back" $(swap_name) $(ok));
    }
}

void qweb_files::task_files_create_download_back(const string &sjson)
{

}

void qweb_files::task_files_finish_download(const string &sjson)
{

}

void qweb_files::task_files_cancel_download_back(const string &sjson)
{

}

void qweb_files::sl_open()
{
    emit sn_open();
}

void qweb_files::sl_message(const string &msg)
{
    char c = check_files_flg(msg);
    if(c == CS_FILES_JSON)
    {
        //解析出json
        string sjson;
        if(get_files_json(msg,sjson))
        {
            //检查json
            string stream;
            string type;
            if(check_json(sjson,stream,type))
            {
                if(stream == _sc_)
                {
                    //执行绑定函数
                    auto it = _map_fn.find(type);
                    if(it != _map_fn.end()) it->second(sjson);
                }

            }
            else vlogw("transmit_msg check_json not find:" $(msg));
        }
        else vloge("err: get_files_json " $(msg));
    }
    else if(c == CS_FILES_BINARY)
    {
        //解析出二进制内容与定位
        int64 id;
        string buf;
        if(get_files_binary(msg,id,buf))
        {

        }
        else vloge("err: get_files_binary " $(msg));
    }
    else vlogw("transmit_msg not find");
}

void qweb_files::sl_close()
{
    emit sn_close();
}
