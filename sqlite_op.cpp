#include "sqlite_op.h"

bool sqlite_base::open_db(cstr file)
{
    return (sqlite3_open(file.c_str(),&_db) == SQLITE_OK ? true : false);
}

bool sqlite_base::close_db()
{
    return (sqlite3_close_v2(_db) == SQLITE_OK ? true : false);
}

bool sqlite_base::delete_db(cstr table)
{
    string sql("DELETE FROM {0};");
    sql = sformat(sql)(table);
    return exec_db(sql);
}

bool sqlite_base::drop_db(cstr table)
{
    string sql = "DROP TABLE IF EXISTS {0};";
    sql = sformat(sql)(table);
    return exec_db(sql);
}

bool sqlite_base::select_db(cstr table, vector<string> &data)
{
    auto fn_cb = [](void *data, int argc, char **argv, char **name){
        vector<string> *vec = (vector<string>*)data;
        string str;
        for (int i=0; i<argc;i++)
        {
            str += " |:" + string(argv[i]);
        }
        vec->push_back(str);
        return 0;
    };

    string sql("SELECT * FROM {0};");
    sql = sformat(sql)(table);
    return exec_db(sql,fn_cb,&data);
}

string sqlite_base::get_error_exec()
{
    return (_error == nullptr ? "" : _error);
}

string sqlite_base::get_error()
{
    return (sqlite3_errmsg(_db) == nullptr ? "" : sqlite3_errmsg(_db));
}

int sqlite_base::count_db(cstr table)
{
    int value = -1;
    sqlite3_stmt *stmt;
    string sql(R"(SELECT COUNT(*) FROM {0};)");
    sql = sformat(sql)(table);

    if(sqlite3_prepare_v2(_db,sql.c_str(),-1,&stmt,nullptr) == SQLITE_OK)
    {
        if(sqlite3_step(stmt) == SQLITE_ROW)
        {
            value = sqlite3_column_int(stmt,0);  //获取结果
        }
    }
    sqlite3_finalize(stmt); //释放预处理语句
    return value;
}

bool sqlite_base::exec_db(cstr sql,int (*cb)(void*,int,char**,char**),void *data)
{
    return (sqlite3_exec(_db,sql.c_str(),cb,data,&_error) == SQLITE_OK ? true : false);
}

sqlite_base::~sqlite_base()
{
    close_db();
}

sqlite_history::sqlite_history()
{
    _data.send_time = "send_time";
    _data.feed_back = "feed_back";
    _data.non_read  = "non_read";
    _data.types     = "types";
    _data.object    = "object";
    _data.content   = "content";
}

bool sqlite_history::open_history(int64 account)
{
    _file = sformat(_file)(account);
    return open_db(_file);
}

bool sqlite_history::create_history(int64 ac_friend)
{
    //! send_time   : ID，发送时间
    //! feed_back   : 接收方反馈确认消息时间（接收方：feed_back == send_time）
    //! non_read    : 未读，0-1,1表示未读，可用于统计未读消息数
    //! types       : 消息类型 [Text,FIle,Img]
    //! object      : 消息对象 [Sys,AL,AR,Hint]
    //! content     : 消息内容 [Text:string,File:abs_path,Img:abs_path]
    string sql (R"(
        CREATE TABLE IF NOT EXISTS {0} (
            {1} INTEGER PRIMARY KEY,
            {2} INTEGER,
            {3} INTEGER CHECK (non_read >= 0 AND non_read <= 1),
            {4} TEXT,
            {5} TEXT,
            {6} TEXT
        );
        )");
    string table = sformat(_table)(ac_friend);
    sql = sformat(sql)(table,
                       _data.send_time ,
                       _data.feed_back ,
                       _data.non_read ,
                       _data.types ,
                       _data.object ,
                       _data.content);
    return exec_db(sql);
}

bool sqlite_history::insert_history(int64 ac_friend, const tuple<int64, int64, int64, string, string, string> &tup)
{
    string table = sformat(_table)(ac_friend);
    return insert_db(table,std::get<0>(tup),std::get<1>(tup),std::get<2>(tup),std::get<3>(tup),std::get<4>(tup),
                     std::get<5>(tup));
}

bool sqlite_history::update_non_read(int64 ac_friend, int64 send_time, int64 non_read)
{
    return update_db(make_table(ac_friend),_data.send_time,send_time,_data.non_read,non_read);
}

bool sqlite_history::update_feed_back(int64 ac_friend, int64 send_time, int64 feed_back)
{
    return update_db(make_table(ac_friend),_data.send_time,send_time,_data.feed_back,feed_back);
}

bool sqlite_history::select_non_read(int64 ac_friend,
        vector<tuple<int64, int64, int64, string, string, string> > &vec_line)
{
    string sql = "SELECT * FROM history_{0} WHERE non_read = 1;";
    sql = sformat(sql)(ac_friend);
    return select_history(ac_friend,vec_line,sql);
}

bool sqlite_history::select_history(int64 ac_friend,
        vector<tuple<int64, int64, int64, string, string, string> > &vec_line, string sql)
{
    bool ok = false;
    if(sql == "")
    {
        sql = "SELECT * FROM history_{0};";
        sql = sformat(sql)(ac_friend);
    }

    vector<map<string,string>> vec;
    if(select_line_db(make_table(ac_friend),vec,"","",sql))
    {
        try{
            for(auto a:vec)
            {
                tuple<int64,int64,int64,string,string,string> tup =
                    std::make_tuple(std::stoll(a[_data.send_time]),
                                    std::stoll(a[_data.feed_back]),
                                    std::stoll(a[_data.non_read]),
                                    a[_data.types],a[_data.object],a[_data.content]);
                vec_line.push_back(tup);
            }
            ok = true;
        }
        catch(...){}
    }
    return ok;
}

string sqlite_history::make_table(int64 ac_friend)
{
    return sformat(_table)(ac_friend);
}

int sqlite_history::count_non_read(int64 ac_friend)
{
    int value = -1;
    sqlite3_stmt *stmt;
    string sql("SELECT SUM(non_read) FROM {0};");
    string table = sformat(_table)(ac_friend);
    sql = sformat(sql)(table);

    if(sqlite3_prepare_v2(_db,sql.c_str(),-1,&stmt,nullptr) == SQLITE_OK)
    {
        if(sqlite3_step(stmt) == SQLITE_ROW)
        {
            value = sqlite3_column_int(stmt,0);  //获取结果
        }
    }
    sqlite3_finalize(stmt); //释放预处理语句
    return value;
}

string sqlite_history::get_file()
{
    return _file;
}

string sqlite_history::get_table()
{
    return _table;
}

sqlite_history::data sqlite_history::get_data()
{
    return _data;
}

sqlite_record::sqlite_record()
{
    _data.time      = "time";
    _data.account   = "account";
    _data.notes     = "notes";
    _data.remarks   = "remarks";
}

bool sqlite_record::open_record(int64 account)
{
    _file = sformat(_file)(account);
    return open_db(_file);
}

bool sqlite_record::create_record(int64 account)
{
    //! time        : 申请时间
    //! account     : 对方账号
    //! notes       : 备注信息
    string sql (R"(
        CREATE TABLE IF NOT EXISTS {0} (
            {1} INTEGER PRIMARY KEY,
            {2} INTEGER,
            {3} TEXT,
            {4} TEXT
        );
        )");
    string table = sformat(_table)(account);
    sql = sformat(sql)(table,
                       _data.time ,
                       _data.account ,
                       _data.notes,
                       _data.remarks);
    return exec_db(sql);
}

bool sqlite_record::insert_record(int64 account,int64 time, int64 friends, string notes,string remarks)
{
    string table = sformat(_table)(account);
    return insert_db(table,time,friends,notes,remarks);
}

bool sqlite_record::delete_record(int64 account, int64 time)
{
    string table = sformat(_table)(account);
    return delete_db(table,_data.time,time);
}

bool sqlite_record::select_record(int64 account, vector<tuple<int64, int64, string,string> > &vec_line)
{
    bool ok = false;
    string sql = "SELECT * FROM {0};";
    sql = sformat(sql)(to_tabel(account));

    vector<map<string,string>> vec;
    if(select_line_db("",vec,"",0,sql))
    {
        try{
            for(auto a:vec)
            {
                tuple<int64,int64,string,string> tup =
                    std::make_tuple(std::stoll(a[_data.time]),
                                    std::stoll(a[_data.account]),
                                    a[_data.notes],
                                    a[_data.remarks]);
                vec_line.push_back(tup);
            }
            ok = true;
        }
        catch(...){}
    }
    return ok;
}

string sqlite_record::get_file()
{
    return _file;
}

string sqlite_record::get_table()
{
    return _table;
}

string sqlite_record::to_tabel(int64 account)
{
    return sformat(_table)(account);
}
