#include "qweb_files.h"

#include "Tvlog.h"

#define ADD_MAP(func) _map_fn.emplace(func,std::bind(&qweb_files::task_##func,this,std::placeholders::_1))

qweb_files::qweb_files(QObject *parent)
    : QObject(parent)
{
    _path_temp_save = "../temp_swap_file/";

    _wc.func_bind(std::bind(&qweb_files::sl_open,this),
                  std::bind(&qweb_files::sl_message,this,std::placeholders::_1),
                  std::bind(&qweb_files::sl_close,this));

    ADD_MAP(files_create_upload_back);
    ADD_MAP(files_finish_upload_back);
    ADD_MAP(files_create_download_back);
    ADD_MAP(files_finish_download);
    ADD_MAP(files_cancel_download_back);


    //文件分段触发回调
    typedef std::shared_ptr<hv::WebSocketChannel> TSocketChannelPtr;
    _wc.get_wc()->onWriteComplete = [=](const TSocketChannelPtr& channel, hv::Buffer* buf) {

        //分段上传结果反馈
        auto fn_finish = [=](bool finish,int64 id) {

            //停止和或者完整传输，完成上传
            bool is_end = false;
            auto it_stop = _map_upload_status.find(id);
            if(it_stop->second.stop || finish)
            {
                _map_upload_status.erase(it_stop);
                is_end = true;
            }

            //完成上传并反馈
            if(is_end)
            {
                string s = set_files_finish_upload(id,is_end);
                send_msg(s);
                return false;
            }
            return true;
        };

        //发送提示：利用ping/pong出发write发送回调，控制发送文件流量
        if(channel->isWriteComplete()
            && _swap_fs.get_send_size_queue() > 0)
        { _swap_fs.start_send_queue(fn_finish); }
    };
}

int qweb_files::open(string ip, int port, string txt)
{
    return _wc.open(ip,port,txt);
}

bool qweb_files::upload_file(int64 time, const string &path)
{
    if(swap_files::is_exists(path) == false
        || swap_files::is_can_read(path) == false
        || swap_files::is_file(path) == false) return false;

    int64 file_size = swap_files::get_file_size(path);
    string filename = swap_files::get_filename(path);
    string s = set_files_create_upload(time,file_size,filename);
    bool ok = send_msg(s);

    fs_status st{false,path};
    if(ok) _map_upload_status.emplace(time,st);
    return ok;
}

bool qweb_files::download_file(int64 id)
{
    string s = set_files_create_download(id);
    return send_msg(s);
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
    if(size >= CS_SENDBUF_HIGH_LINE) return true;
    return false;
}

void qweb_files::task_recv_binary_data(int64 id, const string &data)
{
    bool ok = _swap_fs.add_data_recv(id,data);
    if(ok == false) vlogw("err: task_recv_binary_data ");
}

void qweb_files::task_files_create_upload_back(const string &sjson)
{
    bool is_succeed = false;

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
                auto it_id = _map_upload_status.emplace(id,it->second);
                if(it_id.second)
                {
                    bool ok_open = _swap_fs.open_file_send(id,it->second.filename,func_send);
                    vlogi("open:" $(id) $(ok_open));

                    if(ok_open)
                    {
                        //启动异步发送
                        hv::Buffer buf;
                        _wc.get_wc()->onWriteComplete(_wc.get_wc()->channel,&buf);
                        is_succeed = true;
                    }
                }
                _map_upload_status.erase(it);
            }
        }
    }

    vlogi("task_files_create_upload_back: " $(time) $(id) $(ok) $(is_succeed));

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
    vlogi("task_files_create_download_back");

    int64 swap_id;
    int64 length_max;
    string filename;
    bool ok;
    if(get_files_create_download_back(sjson,swap_id,length_max,filename,ok))
    {
        if(ok)
        {
            string new_name = swap_files::make_none_repeat_file(_path_temp_save,filename);
            string abs_path = _path_temp_save + new_name;
            bool ok_open = _swap_fs.open_file_recv(swap_id,length_max,abs_path);


            vlogd("download_back" $(abs_path) $(ok_open));
            if(ok_open == false) vloge("download_back" $(swap_id));

            string s = set_files_begin_download(swap_id,ok_open);
            send_msg(s);
        }
        vlogd("download_back" $(swap_id) $(length_max) $(ok));
    }
}

void qweb_files::task_files_finish_download(const string &sjson)
{
    int64 swap_id;
    bool ok;
    if(get_files_finish_download(sjson,swap_id,ok))
    {
        if(ok)
        {
            _swap_fs.close_file_recv(swap_id);
            vlogd("finishi: " $(swap_id) $(ok));
        }
        else
        {
            string s = set_files_begin_download(swap_id,true);
            send_msg(s);
//            vlogi("download: " $(swap_id) $(ok));
        }
//        vlogi("task_files_finish_download:" $(swap_id) $(ok));
    }
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
            task_recv_binary_data(id,buf);
        }
        else vloge("err: get_files_binary " $(msg));
    }
    else vlogw("transmit_msg not find");
}

void qweb_files::sl_close()
{
    emit sn_close();
}
