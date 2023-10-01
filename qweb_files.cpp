#include "qweb_files.h"

#include "Tvlog.h"

#define ADD_MAP(func) _map_fn.insert(func,std::bind(&qweb_files::task_##func,this,std::placeholders::_1))

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
}

int qweb_files::open(string ip, int port, string txt)
{
    return _wc.open(ip,port,txt);
}

bool qweb_files::upload_file(int64 time, int64 source, const string &path)
{
    int64 file_size = swap_files::get_file_size(path);
    string filename = swap_files::get_filename(path);
    string s = set_files_create_upload(time,source,source,file_size,filename);
    bool ok = send_msg(s);
    if(ok) _map_upload_filename.insert(time,{true,false,0,path});
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
    return _wc.get_wc()->send(s);
}

bool qweb_files::check_high_line()
{
    if(_wc.get_wc()->channel->writeBufsize() >= WRITE_BUFSIZE_HIGH_WATER)
    {
//        std::this_thread::sleep_for(std::chrono::seconds(3));
//        vlogi("size: " << _wc.get_wc()->channel->writeBufsize());
        return true;
    }
    return false;
}

void qweb_files::task_files_create_upload_back(const string &sjson)
{
    //发送文件数据函数
    auto func_send = [=](bool &stop,int64 id,const string &data){
//        bool ok = check_high_line();
//        if(ok) vlogw("check_high_line" $(ok));

        send_data(id,data);
    };

    //解析反馈
    int64 time;
    int64 swap_name;
    bool ok;
    if(get_files_create_upload_back(sjson,time,swap_name,ok))
    {
        if(ok)
        {
            //查找id定位文件
            auto it = _map_upload_filename.find(time);
            if(it != _map_upload_filename.end())
            {
                //文件发送
                fs_status st = it.value();
                st.swap_name = swap_name;
                bool ret = _swap_fs.send_file(func_send,swap_name,st.filename);

                //完成上传
                string s = set_files_finish_upload(swap_name,st.swap,ret);
                send_msg(s);

            }
            _map_upload_filename.erase(it);
        }
    }

    vlogi($(time) $(swap_name) $(ok));
}

void qweb_files::task_files_finish_upload_back(const string &sjson)
{
    int64 swap_name;
    bool ok;
    if(get_files_finish_upload_back(sjson,swap_name,ok))
    {
        vlogi("get_files_finish_upload_back" $(swap_name) $(ok));
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
                    auto it = _map_fn.value(type,nullptr);
                    if(it != nullptr) it(sjson);
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
