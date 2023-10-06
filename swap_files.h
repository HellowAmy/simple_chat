#ifndef SWAP_FILES_H
#define SWAP_FILES_H

#include <filesystem>
#include <fstream>
#include <string>
#include <functional>
#include <map>
#include <set>
#include <memory>
#include <mutex>
#include <queue>

#include "Tvlog.h"

typedef long long int64;
using std::string;
using std::fstream;
using std::function;
using std::map;
using std::set;
using std::shared_ptr;
using std::mutex;
using std::queue;

typedef std::function<bool(int64 id,const string &data)> Tfn_send_cb;


//class files_info
//{
//public:
//    static bool is_file(const string &path)
//    {  return std::filesystem::is_regular_file(path); }

//    static bool is_exists(const string &path)
//    { return std::filesystem::exists(path); }

//    static bool remove_file(const string &path)
//    { return std::filesystem::remove(path); }

//    static int64 get_file_size(const string &path)
//    { return std::filesystem::file_size(path); }

//    static string get_filename_none(const string &path)
//    {
//        std::filesystem::path p = path;
//        return p.stem();
//    }

//    static string get_filename(const string &path)
//    {
//        std::filesystem::path p = path;
//        return p.filename();
//    }

//    static string get_suffix(const string &path)
//    {
//        std::filesystem::path p = path;
//        return p.extension();
//    }

//    static bool is_can_read(const string &path)
//    {
//        typedef std::filesystem::perms perms;
//        namespace fs = std::filesystem;
//        if((fs::status(path).permissions() & perms::owner_read) != perms::none) return true;
//        return false;
//    }

//    static string make_none_repeat_file(const string &in_path,const string &in_filename)
//    {
//        string filename;
//        string abs_file = in_path+"/"+in_filename;
//        if(is_exists(abs_file))
//        {
//            string file_none = get_filename_none(in_filename);
//            file_none += "_";
//            filename = file_none + get_suffix(in_filename);
//        }
//        else return in_filename;
//        return make_none_repeat_file(in_path,filename);
//    }
//};

//class files_io : public files_info
//{
//public:
//    struct fs_recv
//    {
//        fs_recv(int64 n1,int64 n2,string n3,shared_ptr<fstream> n4)
//                : count_recv(n1),length_max(n2),filename(n3),sp_fs(n4) {}

//        int64 count_recv;           //累加接收
//        int64 length_max;           //文件长度
//        string filename;            //文件名称
//        shared_ptr<fstream> sp_fs;  //写入句柄
//    };

//    struct fs_send
//    {
//        fs_send(int64 n1,int64 n2,string n3,shared_ptr<fstream> n4,Tfn_send_cb n5)
//                : count_send(n1),length_max(n2),filename(n3),sp_fs(n4),fn_send_cb(n5) {}

//        int64 count_send;           //累加发送
//        int64 length_max;           //文件长度
//        string filename;            //文件名称
//        shared_ptr<fstream> sp_fs;  //读取句柄

//        //发送回调：自定义发送文件
//        //参数：[id: 唯一id，用于定位写入流] [data: 读取的数据]
//        //返回值：true时继续发送，false时终止发送，需要重新调用发送函数
//        //typedef std::function<bool(int64 id,const string &data)> Tfn_send_cb;
//        Tfn_send_cb fn_send_cb;
//    };

//public:
//    //== 发送函数 ：多次完成 ==
//    bool open_file_send(int64 id,const string &filename,Tfn_send_cb fn_send_cb)
//    {
//        std::unique_lock<std::mutex> lock(_mut_send);
//        if(_map_send.find(id) != _map_send.end()) return false;

//        shared_ptr sp_fs = std::make_shared<fstream>(filename,std::ios::in | std::ios::binary);
//        if(sp_fs->is_open())
//        {
//            auto it = _map_send.emplace(id,std::make_shared<fs_send>(0,get_file_size(filename),filename,sp_fs,fn_send_cb));
//            return it.second;
//        }
//        else return false;
//    }

//    bool add_data_send(int64 id)
//    {
//        bool is_send = true;
//        auto it =_map_send.find(id);
//        if(it != _map_send.end())
//        {
//            auto sp_send = it->second;

//            char buf[1024];
//            while(sp_send->sp_fs->eof() == false)
//            {
//                {
//                    std::unique_lock<std::mutex> lock(_mut_send);
//                    sp_send->sp_fs->read(buf,sizeof(buf));
//                }
//                size_t size_read = sp_send->sp_fs->gcount();
//                sp_send->count_send += size_read;

//                is_send = sp_send->fn_send_cb(id,string(buf,size_read));
//                if(is_send == false) break;
//            }
//        }
//        return is_send;
//    }

//    void close_file_send(int64 id)
//    {
//        std::unique_lock<std::mutex> lock(_mut_send);
//        auto it = _map_send.find(id);
//        if(it != _map_send.end())
//        {
//            auto sp_send = it->second;
//            sp_send->sp_fs->close();
//            _map_send.erase(it);
//        }
//    }
//    //== 发送函数 ：多次完成 ==



//    //== 接收函数 ：多次完成 ==
//    bool open_file_recv(int64 id,int64 length_max, const string &filename)
//    {
//        std::unique_lock<std::mutex> lock(_mut_recv);
//        if(_map_recv.find(id) != _map_recv.end()) return false;

//        shared_ptr sp_fs = std::make_shared<fstream>(filename,std::ios::out | std::ios::binary);
//        if(sp_fs->is_open())
//        {
//            auto it = _map_recv.emplace(id,std::make_shared<fs_recv>(0, length_max, filename,sp_fs));
//            return it.second;
//        }
//        else return false;
//    }

//    bool add_data_recv(int64 id,const string &data)
//    {
//        auto it = _map_recv.find(id);
//        if(it != _map_recv.end())
//        {
//            auto sp_send = it->second;
//            {
//                std::unique_lock<std::mutex> lock(_mut_recv);
//                sp_send->sp_fs->write(data.c_str(),data.size());
//            }
//            bool ok = sp_send->sp_fs->good();
//            if(ok) sp_send->count_recv += data.size();
//            return ok;
//        }
//        else return false;
//    }

//    void close_file_recv(int64 id)
//    {
//        std::unique_lock<std::mutex> lock(_mut_recv);
//        auto it = _map_recv.find(id);
//        if(it != _map_recv.end())
//        {
//            auto sp_send = it->second;
//            sp_send->sp_fs->close();
//            _map_recv.erase(it);
//        }
//    }
//    //== 接收函数 ：多次完成 ==


//    //== 容器信息 ==
//    std::shared_ptr<fs_recv> find_fs_recv(int64 id)
//    {
//        auto it = _map_recv.find(id);
//        if(it != _map_recv.end())
//        { return it->second; }
//        else return nullptr;
//    }

//    std::shared_ptr<fs_send> find_fs_send(int64 id)
//    {
//        auto it = _map_send.find(id);
//        if(it != _map_send.end())
//        { return it->second; }
//        else return nullptr;
//    }
//    //== 容器信息 ==

//protected:
//    mutex _mut_recv;
//    mutex _mut_send;
//    map<int64,std::shared_ptr<fs_recv>> _map_recv;
//    map<int64,std::shared_ptr<fs_send>> _map_send;
//};

//class files_auto : public files_io
//{
//public:

//    //== 自动发送接口 ==
//    void add_send_queue(int64 id)
//    {
//        std::unique_lock<std::mutex> lock(_mut_send_que);
//        _que_send.push(id);
//    }

//    int64 remove_send_queue()
//    {
//        std::unique_lock<std::mutex> lock(_mut_send_que);
//        int64 id = _que_send.back(); _que_send.pop();
//        return id;
//    }

//    size_t get_send_size_queue()
//    { return _que_send.size(); }

//    // files_io::add_data_send 的限流接口
//    bool add_data_send_limit_flux(int64 id,int64 flux = 2*1024*1024)
//    {
//        bool ok = add_data_send(id);
//        auto it = find_fs_send(id);
//        if(it != nullptr && (it->count_send % flux == 0)) return false;
//        return ok;
//    }

//    //! 自动发送打开的文件数据
//    //!     fn_finish_cb : [finish: 是否完成] [id: 文件标识] [return: 是否继续传输]
//    //!     fn_add_data_send : [id: 文件标识] [return: 是否完成]
//    //!
//    //!     fn_add_data_send 回调可用于 add_data_send 和 add_data_send_limit_flux 两个发送版本
//    //!
//    int64 start_send_queue(std::function<bool(bool finish,int64 id)> fn_finish_cb,
//                           std::function<bool(int64 id)> fn_add_data_send = nullptr)
//    {
//        if(fn_add_data_send == nullptr)
//        { fn_add_data_send = std::bind(&files_auto::add_data_send,this,std::placeholders::_1); }

//        int size_que = _que_send.size();
//        for(int i=0;i<size_que;i++)
//        {
//            int64 id = remove_send_queue();
//            bool ok = fn_add_data_send(id);
//            bool is_send = fn_finish_cb(ok,id);
//            vlogi($(size_que) $(i) $(id));

//            //[未完成：加入队列继续发送] [完成：关闭文件]
//            if(is_send && ok == false) { add_send_queue(id); }
//            else { close_file_send(id); }
//        }
//        vlogi($(size_que) );
//        return _que_send.size();
//    }
//    //== 自动发送接口 ==

//protected:
//    mutex _mut_send_que;
//    queue<int64> _que_send;
//};

//class files_channel : public files_auto
//{
//public:
//    struct fs_channel
//    {
//        fs_channel(set<int64> n1,set<int64> n2) : _set_recv(n1),_set_send(n2) {}
//        set<int64> _set_recv;
//        set<int64> _set_send;
//    };

//public:
//    //== 管理函数--断线时快速关闭文件 ==
//    // files_io::open_file_send 函数的管理版本
//    bool open_file_send_channel(int64 id_channel,int64 id_file,const string &filename,Tfn_send_cb fn_send_cb)
//    {
//        bool ok_open = open_file_send(id_file,filename,fn_send_cb);
//        bool ok_file = add_file_send(id_channel,id_file);

//        if(ok_open && ok_file) return true;
//        else
//        {
//            close_file_send(id_file);
//            remove_file_send(id_channel,id_file);
//            return false;
//        }
//    }

//    // files_io::open_file_recv 函数的管理版本
//    bool open_file_recv_channel(int64 id_channel,int64 id_file,int64 length_max, const string &filename)
//    {
//        bool ok_open = open_file_recv(id_file,length_max,filename);
//        bool ok_file = add_file_recv(id_channel,id_file);
//        if(ok_open && ok_file) return true;
//        else
//        {
//            close_file_recv(id_file);
//            remove_file_recv(id_channel,id_file);
//            return false;
//        }
//    }
//    //== 管理函数--断线时快速关闭文件 ==


//    //== 控制时机 ==
//    //连接开始时调用 , id_channel 为连接唯一标识
//    bool add_channel(int64 id_channel)
//    {
//        std::unique_lock<mutex> lock(_mut_channel);
//        auto it = _map_channel.emplace(id_channel,std::make_shared<fs_channel>
//                (std::set<int64>(),std::set<int64>()));
//        return it.second;
//    }

//    //连接断开时调用
//    void remove_channel(int64 id_channel)
//    {
//        std::unique_lock<mutex> lock(_mut_channel);
//        _map_channel.erase(id_channel);
//    }
//    //== 控制时机 ==


//    //== 文件控制 ==
//    bool add_file_recv(int64 id_channel,int64 id_file)
//    {
//        auto it = _map_channel.find(id_channel);
//        if(it != _map_channel.end())
//        {
//            std::unique_lock<mutex> lock(_mut_channel);
//            it->second->_set_recv.emplace(id_file);
//            return true;
//        }
//        return false;
//    }

//    bool add_file_send(int64 id_channel,int64 id_file)
//    {
//        auto it = _map_channel.find(id_channel);
//        if(it != _map_channel.end())
//        {
//            std::unique_lock<mutex> lock(_mut_channel);
//            it->second->_set_send.emplace(id_file);
//            return true;
//        }
//        return false;
//    }

//    void remove_file_send(int64 id_channel,int64 id_file)
//    {
//        auto it = _map_channel.find(id_channel);
//        if(it != _map_channel.end())
//        {
//            std::unique_lock<mutex> lock(_mut_channel);
//            it->second->_set_send.erase(id_file);
//        }
//    }

//    void remove_file_recv(int64 id_channel,int64 id_file)
//    {
//        auto it = _map_channel.find(id_channel);
//        if(it != _map_channel.end())
//        {
//            std::unique_lock<mutex> lock(_mut_channel);
//            it->second->_set_recv.erase(id_file);
//        }
//    }
//    //== 文件控制 ==

//protected:
//    mutex _mut_channel;
//    map<int64,shared_ptr<fs_channel>> _map_channel;
//};



//!
//! 类说明： 提供发送与接收网络传输文件的读写管理，支持多文件多文件多线程
//!             利用唯一 ID 绑定文件流
//!
class swap_files
{
public:
    struct fs_recv
    {
        fs_recv(int64 n1,int64 n2,string n3,shared_ptr<fstream> n4)
            : count_recv(n1),length_max(n2),filename(n3),sp_fs(n4) {}

        int64 count_recv;           //累加接收
        int64 length_max;           //文件长度
        string filename;            //文件名称
        shared_ptr<fstream> sp_fs;  //写入句柄
    };

    struct fs_send
    {
        fs_send(int64 n1,int64 n2,string n3,shared_ptr<fstream> n4,Tfn_send_cb n5)
            : count_send(n1),length_max(n2),filename(n3),sp_fs(n4),fn_send_cb(n5) {}

        int64 count_send;           //累加发送
        int64 length_max;           //文件长度
        string filename;            //文件名称
        shared_ptr<fstream> sp_fs;  //读取句柄

        //发送回调：自定义发送文件
        //参数：[id: 唯一id，用于定位写入流] [data: 读取的数据]
        //返回值：true时继续发送，false时终止发送，需要重新调用发送函数
        //typedef std::function<bool(int64 id,const string &data)> Tfn_send_cb;
        Tfn_send_cb fn_send_cb;
    };

public:
    swap_files() {}

    static bool is_file(const string &path)
    {  return std::filesystem::is_regular_file(path); }

    static bool is_exists(const string &path)
    { return std::filesystem::exists(path); }

    static bool remove_file(const string &path)
    { return std::filesystem::remove(path); }

    static int64 get_file_size(const string &path)
    { return std::filesystem::file_size(path); }

    static string get_filename_none(const string &path)
    {
        std::filesystem::path p = path;
        return p.stem();
    }

    static string get_filename(const string &path)
    {
        std::filesystem::path p = path;
        return p.filename();
    }

    static string get_extension(const string &path)
    {
        std::filesystem::path p = path;
        return p.extension();
    }

    static bool is_can_read(const string &path)
    {
        typedef std::filesystem::perms perms;
        namespace fs = std::filesystem;
        if((fs::status(path).permissions() & perms::owner_read) != perms::none) return true;
        return false;
    }

    static string make_none_repeat_file(const string &in_path,const string &in_filename)
    {
        string filename;
        string abs_file = in_path+"/"+in_filename;
        if(is_exists(abs_file))
        {
            string file_none = get_filename_none(in_filename);
            file_none += "_";
            filename = file_none + get_extension(in_filename);
        }
        else return in_filename;
        return make_none_repeat_file(in_path,filename);
    }


    //== 发送函数 ：多次完成 ==
    bool open_file_send(int64 id,const string &filename,Tfn_send_cb fn_send_cb,bool ready = true)
    {
        std::unique_lock<std::mutex> lock(_mut_send);
        if(_map_send.find(id) != _map_send.end()) return false;

        shared_ptr sp_fs = std::make_shared<fstream>(filename,std::ios::in | std::ios::binary);
        if(sp_fs->is_open())
        {
            if(ready) add_send_queue(id);
            auto it = _map_send.emplace(id,std::make_shared<fs_send>(0,get_file_size(filename),filename,sp_fs,fn_send_cb));
            return it.second;
        }
        else return false;
    }

    bool add_data_send(int64 id)
    {
        bool is_send = true;
        auto it =_map_send.find(id);
        if(it != _map_send.end())
        {
            auto sp_send = it->second;

            char buf[1024];
            while(sp_send->sp_fs->eof() == false)
            {
                {
                    std::unique_lock<std::mutex> lock(_mut_send);
                    sp_send->sp_fs->read(buf,sizeof(buf));
                }
                size_t size_read = sp_send->sp_fs->gcount();
                sp_send->count_send += size_read;

                is_send = sp_send->fn_send_cb(id,string(buf,size_read));
                if(is_send == false) break;
            }
        }
        return is_send;
    }

    void close_file_send(int64 id)
    {
        std::unique_lock<std::mutex> lock(_mut_send);
        auto it = _map_send.find(id);
        if(it != _map_send.end())
        {
            auto sp_send = it->second;
            sp_send->sp_fs->close();
            _map_send.erase(it);
        }
    }
    //== 发送函数 ：多次完成 ==



    //== 接收函数 ：多次完成 ==
    bool open_file_recv(int64 id,int64 length_max, const string &filename)
    {
        std::unique_lock<std::mutex> lock(_mut_recv);
        if(_map_recv.find(id) != _map_recv.end()) return false;

        shared_ptr sp_fs = std::make_shared<fstream>(filename,std::ios::out | std::ios::binary);
        if(sp_fs->is_open())
        {
            auto it = _map_recv.emplace(id,std::make_shared<fs_recv>(0, length_max, filename,sp_fs));
            return it.second;
        }
        else return false;
    }

    bool add_data_recv(int64 id,const string &data)
    {
        auto it = _map_recv.find(id);
        if(it != _map_recv.end())
        {
            auto sp_send = it->second;
            {
                std::unique_lock<std::mutex> lock(_mut_recv);
                sp_send->sp_fs->write(data.c_str(),data.size());
            }
            bool ok = sp_send->sp_fs->good();
            if(ok) sp_send->count_recv += data.size();
            return ok;
        }
        else return false;
    }

    void close_file_recv(int64 id)
    {
        std::unique_lock<std::mutex> lock(_mut_recv);
        auto it = _map_recv.find(id);
        if(it != _map_recv.end())
        {
            auto sp_send = it->second;
            sp_send->sp_fs->close();
            _map_recv.erase(it);
        }
    }
    //== 接收函数 ：多次完成 ==


    std::shared_ptr<fs_recv> find_fs_recv(int64 id)
    {
        auto it = _map_recv.find(id);
        if(it != _map_recv.end())
        { return it->second; }
        else return nullptr;
    }

    std::shared_ptr<fs_send> find_fs_send(int64 id)
    {
        auto it = _map_send.find(id);
        if(it != _map_send.end())
        { return it->second; }
        else return nullptr;
    }

    void add_send_queue(int64 id)
    {
        std::unique_lock<std::mutex> lock(_mut_send_que);
        _que_send.push(id);
    }

    int64 remove_send_queue()
    {
        std::unique_lock<std::mutex> lock(_mut_send_que);
        int64 id = _que_send.back(); _que_send.pop();
        return id;
    }

    size_t get_send_size_queue()
    { return _que_send.size(); }

    //自动发送打开的文件数据
    //  fn_finish_cb : [finish: 是否完成] [id: 文件标识] [return: 是否继续传输]
    int64 start_send_queue(std::function<bool(bool finish,int64 id)> fn_finish_cb)
    {
        int size_que = _que_send.size();
        for(int i=0;i<size_que;i++)
        {
            int64 id = remove_send_queue();
            bool ok = add_data_send(id);
            bool is_send = fn_finish_cb(ok,id);
            vlogi($(size_que) $(i) $(id));

            //[未完成：加入队列继续发送] [完成：关闭文件]
            if(is_send && ok == false) { add_send_queue(id); }
            else { close_file_send(id); }
        }
        vlogi($(size_que) );
        return _que_send.size();
    }

    void close_map_recv()
    {
        std::unique_lock<std::mutex> lock(_mut_recv);
        for(auto it : _map_recv)
        { it.second->sp_fs->close(); }
        _map_recv.clear();
    }

    void close_map_send()
    {
        {
            std::unique_lock<std::mutex> lock(_mut_send);
            for(auto it : _map_send)
            { it.second->sp_fs->close(); }
            _map_send.clear();
        }
        {
            std::unique_lock<std::mutex> lock(_mut_send_que);
            while(_que_send.size() > 0)
            { _que_send.pop(); }
        }
    }

    void reset()
    {
        close_map_recv();
        close_map_send();
    }

protected:
    mutex _mut_recv;
    mutex _mut_send;
    mutex _mut_send_que;
    map<int64,std::shared_ptr<fs_recv>> _map_recv;
    map<int64,std::shared_ptr<fs_send>> _map_send;
    queue<int64> _que_send;
};

#endif // SWAP_FILES_H
