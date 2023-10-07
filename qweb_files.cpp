#include "qweb_files.h"

#include "Tvlog.h"

#define ADD_MAP(func) _map_fn.emplace(func,std::bind(&qweb_files::task_##func,this,std::placeholders::_1))

qweb_files::qweb_files(QObject *parent)
    : QObject(parent)
{
    _path_temp = "../temp_file/";

    _wc.func_bind(std::bind(&qweb_files::sl_open,this),
                  std::bind(&qweb_files::sl_message,this,std::placeholders::_1),
                  std::bind(&qweb_files::sl_close,this));

    ADD_MAP(files_create_upload_back);
    ADD_MAP(files_begin_upload_back);
    ADD_MAP(files_finish_upload_back);

    ADD_MAP(files_create_download_back);
    ADD_MAP(files_begin_download_back);
    ADD_MAP(files_finish_download_back);

}

int qweb_files::open(string ip, int port, string txt)
{
    return _wc.open(ip,port,txt);
}

bool qweb_files::upload_file(const string &abs_path,const string &save_path)
{
    if(files_info::is_exists(abs_path) == false
        || files_info::is_can_read(abs_path) == false
        || files_info::is_file(abs_path) == false) return false;

    int64 file_size = files_info::get_file_size(abs_path);
    string s = set_files_create_upload(file_size,abs_path,save_path);
    return send_msg(s);
}

bool qweb_files::download_file(const string &abs_path,const string &save_path)
{
    string s = set_files_create_download(abs_path,save_path);
    return send_msg(s);
}

int64 qweb_files::get_id_channel()
{
    return _wc.get_wc()->channel->id();
}

bool qweb_files::send_msg(const string &sjson)
{
    string s = set_files_json(sjson);
    int ret = _wc.get_wc()->send(s);
    if(ret <= 0) vloge("send_msg failed:" << $(sjson));
    return ret > 0;
}

bool qweb_files::send_data(int64 id, const string &msg)
{
    string s = set_files_binary(id,msg);
    int ret = _wc.get_wc()->send(s);
    if(ret <= 0) vloge("send_msg failed:" << $(msg));
    return ret > 0;
}

void qweb_files::task_recv_binary_data(int64 id, const string &data)
{
    bool ok = _fs_swap.add_data_recv(id,data);
    if(ok == false) vlogw("err: task_recv_binary_data ");
}

void qweb_files::task_files_create_upload_back(const string &sjson)
{
    //发送文件数据函数
    auto func_send = [=](int64 id,const string &data){
        return _fs_swap.check_linit_flux(send_data(id,data),id);
    };

    //进度条
    auto func_prog = [=](int64 id,int64 prog,int64 count,int64 length){
        vlogi($(id) $(prog));
    };

    //解析反馈
    int64 id;
    string abs_file;
    bool ok;
    if(get_files_create_upload_back(sjson,id,abs_file,ok))
    {
        if(ok)
        {
            bool ok_open = _fs_swap.open_file_send_channel(get_id_channel(),id,abs_file,func_send,func_prog);
            if(ok_open)
            {
                string s = set_files_begin_upload(id);
                send_msg(s);
            }
            else
            {
                string s = set_files_finish_upload(id,false);
                send_msg(s);
            }

            emit sn_create_upload(ok_open,id,abs_file);
        }

        vlogif(ok,$(abs_file) $(id));
    }
    else vlogw("get_files_create_upload_back");
}

void qweb_files::task_files_begin_upload_back(const string &sjson)
{
    int64 id;
    if(get_files_begin_upload_back(sjson,id))
    {
        //发送数据
        bool ok_send = _fs_swap.add_data_send(id);
        if(ok_send)
        {
            string s = set_files_finish_upload(id,true);
            send_msg(s);
        }
        else
        {
            string s = set_files_begin_upload(id);
            send_msg(s);
        }
    }
    else vlogw("task_files_begin_upload_back");
}

void qweb_files::task_files_finish_upload_back(const string &sjson)
{
    int64 id;
    bool ok;
    if(get_files_finish_upload_back(sjson,id,ok))
    {
        _fs_swap.close_file_send_channel(get_id_channel(),id);
        emit sn_finish_upload(ok,id);

        vlogif(ok,"upload finish: " << $(id) $(ok));
    }
    else vlogw("task_files_begin_upload_back");
}

void qweb_files::task_files_create_download_back(const string &sjson)
{
    //进度条
    auto func_prog = [=](int64 id,int64 prog,int64 count,int64 length){
        vlogi($(id) $(prog));
    };

    int64 id;
    int64 length_file;
    string abs_path;
    string save_path;
    bool ok;
    if(get_files_create_download_back(sjson,id,length_file,abs_path,save_path,ok))
    {
        if(ok)
        {
            //设置保存路径
            string filename;
            if(save_path == "") filename = _path_temp + files_info::get_filename(abs_path);
            else filename = save_path;

            bool ok_open = _fs_swap.open_file_recv_channel(get_id_channel(),id,length_file,filename,func_prog);
            if(ok_open)
            {
                string s = set_files_begin_download(id);
                send_msg(s);
            }
            else
            {
                string s = set_files_finish_download(id,false);
                send_msg(s);
            }

            emit sn_create_download(ok_open,id,abs_path);
        }

        vlogif(ok,$(abs_path) $(save_path) $(length_file) $(id));
    }
    else vlogw("err: task_files_create_download_back");
}

void qweb_files::task_files_begin_download_back(const string &sjson)
{
    int64 id;
    if(get_files_begin_download_back(sjson,id))
    {
        //反馈数据
        string s = set_files_begin_download(id);
        send_msg(s);
    }
    else vlogw("get_files_begin_download_back");
}

void qweb_files::task_files_finish_download_back(const string &sjson)
{
    int64 id;
    bool ok;
    if(get_files_finish_download(sjson,id,ok))
    {
        //检查文件是否完整
        int64 count = 0;
        int64 length = 0;
        bool ok_recv = _fs_swap.check_length_recv(id,count,length);
        bool is_recv_success = false;
        if(ok_recv && ok) is_recv_success = true;

        //关闭文件
        _fs_swap.close_file_recv_channel(get_id_channel(),id);

        //接收反馈
        string s = set_files_finish_download(id,true);
        send_msg(s);

        //下载通知
        emit sn_finish_download(is_recv_success,id);

        vlogif(is_recv_success,$(ok) $(id) $(is_recv_success) $(ok_recv) $(count) $(length));
    }
    else vlogw("get_files_finish_download");
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
    _fs_swap.remove_id_channel(get_id_channel());

    emit sn_close();
}
