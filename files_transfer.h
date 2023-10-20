#ifndef FILES_TRANSFER_H
#define FILES_TRANSFER_H

#include <filesystem>
#include <fstream>
#include <string>
#include <functional>
#include <map>
#include <set>
#include <memory>
#include <mutex>
#include <queue>
#include <atomic>

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
typedef std::function<void(int64 id,int64 prog,int64 count,int64 length)> Tfn_prog_cb;

//!
//! 类说明：查看文件信息
//!
class files_info
{
public:
    static bool is_file(const string &path)
    {  return std::filesystem::is_regular_file(path); }

    static bool is_exists(const string &path)
    { return std::filesystem::exists(path); }

    static bool remove_file(const string &path)
    { return std::filesystem::remove(path); }

    static bool copy_file(const string &from,const string &to)
    { return std::filesystem::copy_file(from,to,std::filesystem::copy_options::overwrite_existing); }

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

    static string get_suffix(const string &path)
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
            filename = file_none + get_suffix(in_filename);
        }
        else return in_filename;
        return make_none_repeat_file(in_path,filename);
    }
};

//!
//! 类说明：文件读写控制
//!
class files_io : public files_info
{
public:

    //!
    //! 进度反馈：反馈发送文件的进度
    //!     参数：[id: 唯一id，用于定位写入流] [prog: 进度白分比]
    //!     typedef std::function<void(int64 id,short prog)> Tfn_prog_cb;
    //!
    //! 发送回调：自定义发送文件
    //!     参数：[id: 唯一id，用于定位写入流] [data: 读取的数据]
    //!     返回值：true时继续发送，false时终止发送，需要重新调用发送函数
    //!     typedef std::function<bool(int64 id,const string &data)> Tfn_send_cb;
    //!
    struct fs_recv
    {
        fs_recv(int64 n1,int64 n2,int64 n3,string n4,shared_ptr<fstream> n5,Tfn_prog_cb n6)
            : prog(n1),count_recv(n2),length_max(n3),filename(n4),sp_fs(n5),prog_cb(n6) {}

        int64 prog;                 //进度记录
        int64 count_recv;           //累加接收
        int64 length_max;           //文件长度
        string filename;            //文件名称
        shared_ptr<fstream> sp_fs;  //写入句柄
        Tfn_prog_cb prog_cb;        //进度反馈
    };

    struct fs_send
    {
        fs_send(int64 n1,int64 n2,int64 n3,string n4,shared_ptr<fstream> n5,Tfn_prog_cb n6,Tfn_send_cb n7)
            : prog(n1),count_send(n2),length_max(n3),filename(n4),sp_fs(n5),prog_cb(n6),fn_send_cb(n7) {}

        int64 prog;                 //进度记录
        int64 count_send;           //累加发送
        int64 length_max;           //文件长度
        string filename;            //文件名称
        shared_ptr<fstream> sp_fs;  //读取句柄
        Tfn_prog_cb prog_cb;        //进度回调
        Tfn_send_cb fn_send_cb;     //发送回调
    };

public:
    //== 发送函数 ：多次完成 ==
    bool open_file_send(int64 id,const string &filename,Tfn_send_cb fn_send_cb,Tfn_prog_cb prog_cb = nullptr)
    {
        std::unique_lock<std::mutex> lock(_mut_send);
        if(_map_send.find(id) != _map_send.end()) return false;

        shared_ptr sp_fs = std::make_shared<fstream>(filename,std::ios::in | std::ios::binary);
        if(sp_fs->is_open())
        {
            int64 length = get_file_size(filename);
            auto it = _map_send.emplace(id,std::make_shared<fs_send>(0,0,length,filename,sp_fs,prog_cb,fn_send_cb));
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
                int prog = sp_send->count_send * 100 / sp_send->length_max;
                if(prog > sp_send->prog)
                {
                    if(sp_send->prog_cb) sp_send->prog_cb(id,prog,sp_send->count_send,sp_send->length_max);
                    sp_send->prog = prog;
                }
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
    bool open_file_recv(int64 id,int64 length_max, const string &filename,Tfn_prog_cb prog_cb = nullptr)
    {
        std::unique_lock<std::mutex> lock(_mut_recv);
        if(_map_recv.find(id) != _map_recv.end()) return false;

        shared_ptr sp_fs = std::make_shared<fstream>(filename,std::ios::out | std::ios::binary);
        if(sp_fs->is_open())
        {
            auto it = _map_recv.emplace(id,std::make_shared<fs_recv>(0,0,length_max,filename,sp_fs,prog_cb));
            return it.second;
        }
        else return false;
    }

    bool add_data_recv(int64 id,const string &data)
    {
        auto it = _map_recv.find(id);
        if(it != _map_recv.end())
        {
            auto sp_recv = it->second;
            {
                std::unique_lock<std::mutex> lock(_mut_recv);
                sp_recv->sp_fs->write(data.c_str(),data.size());
            }
            bool ok = sp_recv->sp_fs->good();
            if(ok) sp_recv->count_recv += data.size();

            int prog = sp_recv->count_recv * 100 / sp_recv->length_max;
            if(prog > sp_recv->prog)
            {
                if(sp_recv->prog_cb) sp_recv->prog_cb(id,prog,sp_recv->count_recv,sp_recv->length_max);
                sp_recv->prog = prog;
            }
            return ok;
        }
        return false;
    }

    void close_file_recv(int64 id)
    {
        std::unique_lock<std::mutex> lock(_mut_recv);
        auto it = _map_recv.find(id);
        if(it != _map_recv.end())
        {
            auto sp_recv = it->second;
            sp_recv->sp_fs->close();
            _map_recv.erase(it);
        }
    }
    //== 接收函数 ：多次完成 ==


    //== 容器信息 ==
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

    bool check_length_recv(int64 id,int64 &count,int64 &length)
    {
        auto it = find_fs_recv(id);
        if(it != nullptr)
        {
            count = it->count_recv;
            length = it->length_max;
            if(it->count_recv == it->length_max) return true;
        }
        return false;
    }

    bool check_length_send(int64 id,int64 &count,int64 &length)
    {
        auto it = find_fs_send(id);
        if(it != nullptr)
        {
            count = it->count_send;
            length = it->length_max;
            if(it->count_send == it->length_max) return true;
        }
        return false;
    }
    //== 容器信息 ==

protected:
    mutex _mut_recv;
    mutex _mut_send;
    map<int64,std::shared_ptr<fs_recv>> _map_recv;
    map<int64,std::shared_ptr<fs_send>> _map_send;
};


//!
//! 类说明：统一控制网络连接的文件读写
//!
class files_channel : public files_io
{
public:
    struct fs_channel
    {
        fs_channel(set<int64> n1,set<int64> n2) : _set_recv(n1),_set_send(n2) {}
        set<int64> _set_recv;
        set<int64> _set_send;
    };

public:
    //检查连续发送范围是否越界
    bool check_linit_flux(bool ok,int64 id,int64 flux = 2*1024*1024)
    {
        auto it = find_fs_send(id);
        if(it != nullptr && (it->count_send % flux == 0))
        { return false; }
        return ok;
    }

    //== 管理函数--断线时快速关闭文件 ==
    // files_io::open_file_send 函数的管理版本
    bool open_file_send_channel(int64 id_channel,int64 id_file,const string &filename,
                                Tfn_send_cb fn_send_cb,Tfn_prog_cb prog_cb = nullptr)
    {
        bool ok_open = open_file_send(id_file,filename,fn_send_cb,prog_cb);
        bool ok_file = add_file_send(id_channel,id_file);

        if(ok_open && ok_file) return true;
        else
        {
            close_file_send(id_file);
            remove_file_send(id_channel,id_file);
            return false;
        }
    }

    // files_io::open_file_recv 函数的管理版本
    bool open_file_recv_channel(int64 id_channel,int64 id_file,int64 length_max, const string &filename,
                                Tfn_prog_cb prog_cb = nullptr)
    {
        bool ok_open = open_file_recv(id_file,length_max,filename,prog_cb);
        bool ok_file = add_file_recv(id_channel,id_file);
        if(ok_open && ok_file) return true;
        else
        {
            close_file_recv(id_file);
            remove_file_recv(id_channel,id_file);
            return false;
        }
    }

    void close_file_send_channel(int64 id_channel,int64 id_file)
    {
        close_file_send(id_file);
        remove_file_send(id_channel,id_file);
    }

    void close_file_recv_channel(int64 id_channel,int64 id_file)
    {
        close_file_recv(id_file);
        remove_file_recv(id_channel,id_file);
    }
    //== 管理函数--断线时快速关闭文件 ==


    //== 开启控制范围 ==
    //连接开始时调用 , id_channel 为连接唯一标识
    bool add_id_channel(int64 id_channel)
    {
        std::unique_lock<mutex> lock(_mut_channel);
        auto it = _map_channel.emplace(id_channel,std::make_shared<fs_channel>
                                       (std::set<int64>(),std::set<int64>()));
        return it.second;
    }

    //连接断开时调用
    void remove_id_channel(int64 id_channel)
    {
        std::unique_lock<mutex> lock(_mut_channel);
        _map_channel.erase(id_channel);
    }
    //== 开启控制范围 ==


    //== 文件控制 ==
    bool add_file_recv(int64 id_channel,int64 id_file)
    {
        auto it = _map_channel.find(id_channel);
        if(it != _map_channel.end())
        {
            std::unique_lock<mutex> lock(_mut_channel);
            it->second->_set_recv.emplace(id_file);
            return true;
        }
        return false;
    }

    bool add_file_send(int64 id_channel,int64 id_file)
    {
        auto it = _map_channel.find(id_channel);
        if(it != _map_channel.end())
        {
            std::unique_lock<mutex> lock(_mut_channel);
            it->second->_set_send.emplace(id_file);
            return true;
        }
        return false;
    }

    void remove_file_send(int64 id_channel,int64 id_file)
    {
        auto it = _map_channel.find(id_channel);
        if(it != _map_channel.end())
        {
            std::unique_lock<mutex> lock(_mut_channel);
            it->second->_set_send.erase(id_file);
        }
    }

    void remove_file_recv(int64 id_channel,int64 id_file)
    {
        auto it = _map_channel.find(id_channel);
        if(it != _map_channel.end())
        {
            std::unique_lock<mutex> lock(_mut_channel);
            it->second->_set_recv.erase(id_file);
        }
    }

    map<int64,shared_ptr<fs_channel>> get_channel_map()
    { return _map_channel; }
    //== 文件控制 ==

protected:
    mutex _mut_channel;
    map<int64,shared_ptr<fs_channel>> _map_channel;
};


#endif // FILES_TRANSFER_H
