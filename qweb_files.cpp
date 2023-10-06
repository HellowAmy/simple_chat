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
            _fs_swap.close_file_send_channel(get_id_channel(),id);

            string s = set_files_finish_upload(id,is_end);
            send_msg(s);
            return false;
        }
        return true;
    };

    //文件发送，分段触发写入回调
    typedef std::shared_ptr<hv::WebSocketChannel> TSocketChannelPtr;
    _wc.get_wc()->onWriteComplete = [=](const TSocketChannelPtr& channel, hv::Buffer* buf) {

        //发送提示：利用ping/pong出发write发送回调，控制发送文件流量
        if(channel->isWriteComplete()
            && (_fs_swap.get_sending() == false)
            && (_fs_swap.get_send_size_queue() > 0))
        {
            _fs_swap.start_send_queue(fn_finish);
            vlogi("send part");
        }
    };
}

int qweb_files::open(string ip, int port, string txt)
{
    return _wc.open(ip,port,txt);
}

bool qweb_files::upload_file(int64 time, const string &path)
{
    if(files_info::is_exists(path) == false
        || files_info::is_can_read(path) == false
        || files_info::is_file(path) == false) return false;

    int64 file_size = files_info::get_file_size(path);
    string filename = files_info::get_filename(path);
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

int64 qweb_files::get_id_channel()
{
    return _wc.get_wc()->channel->id();
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
    if(_wc.get_wc()->channel->writeBufsize() > 0) return true;
    return false;
}

void qweb_files::task_recv_binary_data(int64 id, const string &data)
{
    bool ok = _fs_swap.add_data_recv(id,data);
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
                    bool ok_open = _fs_swap.open_file_send_channel(get_id_channel(),id,it->second.filename,func_send);
                    _fs_swap.add_send_queue(id);
//                    bool ok_open = _swap_fs.open_file_send(id,it->second.filename,func_send);
//                    vlogi("open:" $(id) $(ok_open));
                    vlogif(ok_open,$(id) $(ok_open) $(get_id_channel()));

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
            string new_name = files_info::make_none_repeat_file(_path_temp_save,filename);
            string abs_path = _path_temp_save + new_name;
            bool ok_open = _fs_swap.open_file_recv_channel(get_id_channel(),swap_id,length_max,abs_path);
//            bool ok_open = _swap_fs.open_file_recv(swap_id,length_max,abs_path);

            vlogif(ok_open, $(abs_path) $(ok_open));

//            vlogd("download_back" $(abs_path) $(ok_open));
//            if(ok_open == false) vloge("download_back" $(swap_id));

            string s = set_files_begin_download(swap_id,ok_open);
            send_msg(s);
        }

        vlogif(ok,$(filename) $(length_max) $(swap_id));
//        vlogd("download_back" $(swap_id) $(length_max) $(ok));
    }
    else vlogw("err: task_files_create_download_back");
}

void qweb_files::task_files_finish_download(const string &sjson)
{
    int64 swap_id;
    bool ok;
    if(get_files_finish_download(sjson,swap_id,ok))
    {
        if(ok)
        {
            int64 count;
            int64 length;
            bool ok_recv = _fs_swap.check_length_recv(swap_id,&count,&length);
            _fs_swap.close_file_recv_channel(get_id_channel(),swap_id);

//            _swap_fs.close_file_recv(swap_id);
            vlogif(ok_recv, $(swap_id) $(ok) $(ok_recv) $(count) $(length));
//            vlogd("finishi: " $(swap_id) $(ok) $(ok_recv));
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
    bool ok = _fs_swap.add_id_channel(get_id_channel());

    vlogif(ok,$(ok) $(get_id_channel()));
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
    auto it = _fs_swap.get_channel_map();
    vlogi($(it.size()));
    for(auto a:it)
    {
        shared_ptr<files_channel::fs_channel> sp = a.second;
        vlogi($(sp->_set_recv.size()) $(sp->_set_send.size()));
    }


    _fs_swap.remove_id_channel(get_id_channel());
    _map_upload_status.clear();

    emit sn_close();
}
