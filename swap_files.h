#ifndef SWAP_FILES_H
#define SWAP_FILES_H

#include <filesystem>
#include <fstream>
#include <string>
#include <functional>
#include <map>
#include <memory>
#include <mutex>

#include "Tvlog.h"

typedef long long int64;
using std::string;
using std::fstream;
using std::function;
using std::map;
using std::shared_ptr;
using std::mutex;

typedef std::function<bool(int64 id,const string &data)> Tfn_send_cb;

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
    bool open_file_send(int64 id,const string &filename,Tfn_send_cb fn_send_cb)
    {
        std::unique_lock<std::mutex> lock(_mut_send);
        if(_map_send.find(id) != _map_send.end()) return false;

        shared_ptr sp_fs = std::make_shared<fstream>(filename,std::ios::in | std::ios::binary);
        if(sp_fs->is_open())
        {
            _map_send.emplace(id,std::make_shared<fs_send>(0, get_file_size(filename), filename,sp_fs,fn_send_cb));
            return true;
        }
        else return false;
    }

    bool add_data_send(int64 id,bool &is_send)
    {
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
        else return false;

        return true;
    }

    void close_file_send(int64 id)
    {
        std::unique_lock<std::mutex> lock(_mut_send);
        auto it = _map_send.find(id);
        if(it != _map_send.end())
        {
            auto sp_send = it->second;
            sp_send->sp_fs->close();
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
            _map_recv.emplace(id,std::make_shared<fs_recv>(0, length_max, filename,sp_fs));
            return true;
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

protected:
    mutex _mut_recv;
    mutex _mut_send;
    map<int64,std::shared_ptr<fs_recv>> _map_recv;
    map<int64,std::shared_ptr<fs_send>> _map_send;
};

#endif // SWAP_FILES_H
