#ifndef SQLITE_OP_H
#define SQLITE_OP_H


#include <string>
#include <vector>
#include <tuple>
#include <map>
#include <functional>

#include "format.h"
#include "include/sqlist3/sqlite3.h"
//#include "../include/sqlite3/sqlite3.h"

//#include "../util/Tvlog.h"

typedef const std::string& cstr;
typedef long long int64;
using std::string;
using std::map;
using std::vector;
using std::tuple;

//
class sqlite_base
{
public:
    //通用操作
    ~sqlite_base();
    bool open_db(cstr file);                                    //打开
    bool close_db();                                            //关闭
    bool drop_db(cstr table);                                   //删除表
    bool delete_db(cstr table);                                 //删除数据
    bool select_db(cstr table, vector<string> &data);           //查数据

    int count_db(cstr table);   //统计条目
    string get_error_exec();    //错误显示
    string get_error();         //错误显示

    //删除单条
    template <class Tid>
    bool delete_db(cstr table, cstr field_id, Tid id)
    {
        string sql(R"( DELETE FROM {0} WHERE {1} = {2}; )");
        sql = sformat(sql)(table,field_id,
                           (is_string(id) ? "'{0}'" : "{0}"));
        sql = sformat(sql)(id);
        return exec_db(sql);
    }

    //修改数据
    template <class Tid,class Tupdata>
    bool update_db(cstr table, cstr field_id, Tid id, cstr field_update, Tupdata update)
    {
        string sql(R"( UPDATE {0} SET {1} = {2} WHERE {3} = {4}; )");
        sql = sformat(sql)(table,
                           field_update,
                           (is_string(update) ? "'{0}'" : "{0}"),
                           field_id,
                           (is_string(id) ? "'{1}'" : "{1}"));
        sql = sformat(sql)(update,id);
        return exec_db(sql);
    }

    //插入数据 : [ table , value1 , value2 , ...]
    template <class... Tarr>
    bool insert_db(cstr table,Tarr... args)
    {
        string sql(R"(INSERT INTO {0} VALUES ({1});)");
        sql = sformat(sql)(table,"{0}");
        return insert_mpl(sql,args...);
    }

    //按行选择
    template<class Tid>
    bool select_line_db(cstr table,vector<map<string,string>> &_vec_line,
                        cstr field_id, Tid id,
                        string sql = "")
    {
        auto fn_cb = [](void *data, int argc, char **argv, char **name){
            auto vec = (vector<map<string,string>>*)data;
            map<string,string> map;
            for (int i=0; i<argc;i++)
            { map[name[i]] = argv[i]; }
            vec->push_back(map);
            return 0;
        };

        if(sql == "")
        {
            sql = "SELECT * FROM {0} WHERE {1} = {2};";
            sql = sformat(sql)(table,field_id,id);
        }
        return exec_db(sql,fn_cb,&_vec_line);
    }


protected:
    sqlite3 *_db = nullptr;
    char* _error = nullptr;

    template <class T>
    bool is_string(T value)
    {
        if(typeid(value) == typeid(string)
            || typeid(value) == typeid(const char *)) return true;
        return false;
    }

    bool exec_db(cstr sql,int (*cb)(void*,int,char**,char**) = nullptr,void *data = nullptr);

    //== 插入数据 : 具体操作 ==
    template <class T>
    string insert_parse_sql(cstr sql,T value,bool app = true)
    {
        //判断数字与字符调整插入选项
        string flg;
        if(typeid(T) == typeid(string)
            || typeid(T) == typeid(const char*) )
        { flg = "'{0}'"; }
        else flg = "{0}";

        //插入并判断是否追加后续
        flg = sformat(flg)(value);
        if(app) flg += ",{0}";
        return sformat(sql)(flg);
    }

    template <class T,class... Tarr>
    bool insert_mpl(string sql,T value)
    {
        sql = insert_parse_sql(sql,value,false);
        return exec_db(sql);
    }

    template <class T, class... Tarr>
    bool insert_mpl(string sql,T value,Tarr... args)
    {
        sql = insert_parse_sql(sql,value);
        return insert_mpl(sql,args...);
    }
    //== 插入数据 : 具体操作 ==
};


class sqlite_history : public sqlite_base
{
public:
    struct data
    {
        string send_time;
        string feed_back;
        string non_read;
        string types;
        string object;
        string content;
    };

public:
    sqlite_history();
    bool open_history(int64 account);
    bool create_history(int64 ac_friend);
    bool insert_history(int64 ac_friend,const tuple<int64, int64, int64, string, string, string> &tup);
    bool update_non_read(int64 ac_friend,int64 send_time, int64 non_read);
    bool update_feed_back(int64 ac_friend,int64 send_time, int64 feed_back);
    bool select_non_read(int64 ac_friend,vector<tuple<int64, int64, int64, string, string, string>> &vec_line);
    bool select_history(int64 ac_friend,vector<tuple<int64, int64, int64, string, string, string>> &vec_line,string sql = "");

    string make_table(int64 ac_friend);


    int count_non_read(int64 ac_friend);

    string get_file();
    string get_table();
    data get_data();

private:
    string _file = "../data/history_{0}.db";
    string _table = "history_{0}";
    data _data;
};

//class sqlite_op
//{
//public:
//    sqlite_op();
//};

#endif // SQLITE_OP_H
