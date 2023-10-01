#ifndef SWAP_FILES_H
#define SWAP_FILES_H

#include <filesystem>
#include <fstream>
#include <string>
#include <functional>
#include <map>


#include "Tvlog.h"
//#include "inter_client.h"

typedef long long int64;
using std::string;
using std::fstream;
using std::function;
using std::map;

class swap_files
{
public:
    struct fs_status
    {
        int64 length_max;
        string filename;
        fstream *fs;
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

    //发送函数，一次性完成
    bool send_file(std::function<void(bool&,int64,const string&)> func_send, int64 id,const string &filename)
    {
        bool is_send = true;
        if(is_file(filename) == false) return false;
        if(is_can_read(filename) == false) return false;

        fstream fs(filename,std::ios::in | std::ios::binary);

        if(fs.is_open())
        {
            char buf[1024];
            size_t count = 0;

            while(fs.eof() == false && is_send)
            {
                fs.read(buf,sizeof(buf));
                size_t size_read = fs.gcount();
                count += size_read;

                func_send(is_send,id,string(buf,size_read));
                if(is_send == false) break;
            }
            fs.close();
        }

        return is_send;
    }

    //== 接收函数 ： 多次完成 ==
    bool open_recv_file(int64 id,int64 length_max,const string &filename)
    {
        if(_map_read.find(id) != _map_read.end()) return false;

        fs_status *fst = new fs_status;
        fst->length_max = length_max;
        fst->filename = filename;
        fst->fs = new fstream(filename,std::ios::out | std::ios::binary);

        auto it = _map_read.emplace(id,fst);
        return it.second;
    }

    bool add_recv_buf(int64 id,const string &data)
    {
        bool is_revc = true;

        auto it = _map_read.find(id);
        if(it != _map_read.end())
        {
            fs_status *fst = it->second;
            fst->fs->write(data.c_str(),data.size());
            is_revc = fst->fs->good();
        }
        else is_revc = false;

        return is_revc;
    }

    fstream* find_fs(int64 id)
    {
        auto it = _map_read.find(id);
        if(it != _map_read.end())
        {
            fs_status *fst = it->second;
            return fst->fs;
        }
        else return nullptr;
    }

    void close_recv_buf(int64 id)
    {
        auto it = _map_read.find(id);
        if(it != _map_read.end())
        {
            fs_status *fst = it->second;
            fst->fs->close();
        }
        move_recv_map(id);
    }

    void move_recv_map(int64 id)
    {
        auto it =_map_read.find(id);
        if(it != _map_read.end())
        {
            fs_status *fst = it->second;
            delete fst->fs;
            delete fst;
            _map_read.erase(it);
        }
    }
    //== 接收函数 ： 多次完成 ==

protected:
    map<int64,fs_status*> _map_read;
};

#endif // SWAP_FILES_H
