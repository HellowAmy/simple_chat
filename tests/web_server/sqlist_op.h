#ifndef SQLIST_OP_H
#define SQLIST_OP_H

#include <string>
#include <vector>

#include "format.h"
#include "../../include/sqlist3/sqlite3.h"

using std::string;
using std::vector;

class sqlist_abs
{
public:
    //通用操作
    bool open_db(string file);      //打开
    bool close_db();                //关闭
    bool delete_db(string table);   //删除数据
    bool drop_db(string table);     //删除表
    bool select_db(string table,vector<string> &data);   //查数据

    string get_error_exec();   //错误显示
    string get_error();        //错误显示

protected:
    sqlite3 *_db = nullptr;
    char* _error = nullptr;
};

//ac passwd
class sqlist_account : public sqlist_abs
{
public:
    struct data_account
    {
        string account;
        string passwd;
    };

public:
    sqlist_account();

    bool open_account();
    bool create_account();
    bool insert_account(uint account,string passwd);
    bool update_account(uint account,string passwd);
    bool delete_account(uint account);

    data_account* get_data();
    string get_file();
    string get_table();

private:
    string _file = "ac_passwd.db";
    string _table = "ac_passwd";
    data_account _data;
};

#endif // SQLIST_OP_H
