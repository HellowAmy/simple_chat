#include "qsqlist.h"
#include "Tvlog.h"

#include <sstream>

#define qtoc(s) s.toStdString().c_str()

template<typename T>
string to_string(const T& t){ std::ostringstream os; os<<t; return os.str(); }

template<typename T>
T from_string(const string& str){ T t; std::istringstream iss(str); iss>>t; return t; }


qsqlist_base::qsqlist_base(QObject *parent)
    : QObject(parent)
{

}

qsqlist_base::~qsqlist_base()
{
    close_db();
}

bool qsqlist_base::open_db(QString file)
{
    return (sqlite3_open(file.toStdString().c_str(),&_db) == SQLITE_OK ? true : false);
}

int qsqlist_base::create_test(QString table)
{
    QString sql (R"(
        CREATE TABLE IF NOT EXISTS %1 (
            id INTEGER PRIMARY KEY,
            value TEXT
        );
        )");
    sql = sql.arg(table);
    return sqlite3_exec(_db,qtoc(sql),nullptr,nullptr,&_error);
}

bool qsqlist_base::drop_db(QString table)
{
    QString sql = "DROP TABLE IF EXISTS %1;";
    sql = sql.arg(table);
    return (sqlite3_exec(_db,qtoc(sql),nullptr,nullptr,&_error) == SQLITE_OK ? true : false);
}

bool qsqlist_base::select_db(QString table, QVector<QString> &data)
{
    auto fn_cb = [](void *data, int argc, char **argv, char **name){
        QVector<QString> *vec = (QVector<QString>*)data;
        QString str;
        for (int i=0; i<argc;i++)
        {
            str += " |:" + QString(argv[i]);
        }
        vec->push_back(str);
        return 0;
    };

    QString sql("SELECT * FROM %1;");
    sql = sql.arg(table);
    return (sqlite3_exec(_db,qtoc(sql),fn_cb,&data,&_error) == SQLITE_OK ? true : false);
}

int qsqlist_base::insert_test(QString table, const tuple<int, string> &data)
{
    QString sql (R"(INSERT INTO %1 VALUES (%2,'%3');)");
    sql = sql.arg(table).arg(std::get<0>(data))
              .arg(QString::fromStdString(std::get<1>(data)));
    return sqlite3_exec(_db,qtoc(sql),nullptr,nullptr,&_error);
}

int qsqlist_base::select_test(QString table,QVector<tuple<int, string>> &data)
{
    auto fn_cb = [](void *data, int argc, char **argv, char **name){
        QVector<tuple<int,string>> *vec =(QVector<tuple<int,string>>*)data;
        int id;
        string value;
        for (int i=0; i<argc;i++)
        {
            if(QString(name[i]) == "id") id = from_string<int>(argv[i]);
            else if(QString(name[i]) == "value") value = argv[i];
        }
        tuple<int,string> t = std::make_tuple(id,value);
        vec->push_back(t);
        return 0;
    };

    QString sql("SELECT * FROM %1;");
    sql = sql.arg(table);
    return sqlite3_exec(_db,qtoc(sql),fn_cb,&data,&_error);
}

bool qsqlist_base::close_db()
{
    return (sqlite3_close_v2(_db) == SQLITE_OK ? true : false);
}

QString qsqlist_base::get_error_exec()
{
    return QString(_error);
}

QString qsqlist_base::get_error()
{
    return QString(sqlite3_errmsg(_db));
}

bool qsqlist_base::delete_db(QString table)
{
    QString sql("DELETE FROM %1;");
    sql = sql.arg(table);
    return (sqlite3_exec(_db,qtoc(sql),nullptr,nullptr,&_error) == SQLITE_OK ? true : false);
}

qsqlist_history::qsqlist_history(QObject *parent)
{
    _name.send_time = "send_time";
    _name.feed_back = "feed_back";
    _name.non_read  = "non_read";
    _name.types     = "types";
    _name.object    = "object";
    _name.content   = "content";
}

bool qsqlist_history::open_history(uint account)
{
    _name_history = _name_history.arg(account);
    return open_db(_name_history);
}

bool qsqlist_history::create_history(uint account)
{
    //! send_time   : ID，发送时间
    //! feed_back   : 接收方反馈确认消息时间（接收方：feed_back == send_time）
    //! non_read    : 未读，0-1,1表示未读，可用于统计未读消息数
    //! type        : 消息类型,>0,类型范围未定，用于判断消息类型，      0：提示消息，1：文字，2：图片，3：文件
    //! object      : 发送消息对象，0-3,                           0：系统消息，1：我方，2：对方，3：提示消息
    //! content     : 记录消息内容的字符串，                       文字：内容，图片：路径，文件：路径
    QString sql (R"(
        CREATE TABLE IF NOT EXISTS history_%1 (
            %2 INTEGER PRIMARY KEY,
            %3 INTEGER CHECK (feed_back >= send_time),
            %4 INTEGER CHECK (non_read >= 0 AND non_read <= 1),
            %5 INTEGER CHECK (types >= 0),
            %6 INTEGER CHECK (object >= 0 AND object <= 3),
            %7 TEXT
        );
        )");
    sql = sql.arg(account)
                .arg(_name.send_time)
                .arg(_name.feed_back)
                .arg(_name.non_read)
                .arg(_name.types)
                .arg(_name.object)
                .arg(_name.content);
    return (sqlite3_exec(_db,qtoc(sql),nullptr,nullptr,&_error) == SQLITE_OK ? true : false);
}

bool qsqlist_history::insert_history(uint account,const tuple<uint, uint, uint, uint, uint, string> &data)
{
    QString sql (R"(INSERT INTO history_%1 VALUES (%2,%3,%4,%5,%6,'%7');)");
    sql = sql.arg(account)
              .arg(std::get<0>(data))
              .arg(std::get<1>(data))
              .arg(std::get<2>(data))
              .arg(std::get<3>(data))
              .arg(std::get<4>(data))
              .arg(QString::fromStdString(std::get<5>(data)));
    return (sqlite3_exec(_db,qtoc(sql),nullptr,nullptr,&_error) == SQLITE_OK ? true : false);
}

bool qsqlist_history::update_history(uint account,uint id, QString name, QVariant value)
{
    sqlite3_stmt *stmt;
    QString sql("UPDATE history_%1 SET %2 = %3 WHERE %4 = %5;");

    if(value.type() ==  QVariant::Int || value.type() ==  QVariant::UInt)
    {
        sql = sql.arg(account).arg(name).arg(value.toUInt()).arg(_name.send_time).arg(id);
    }
    else if(value.type() ==  QVariant::String)
    {
        QString flg("'%1'");
        flg = flg.arg(value.toString());
        sql = sql.arg(account).arg(name).arg(flg).arg(_name.send_time).arg(id);
    }

    if(sqlite3_prepare_v2(_db,qtoc(sql),-1,&stmt,nullptr) != SQLITE_OK) return false;

    int ret = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (ret == SQLITE_DONE ? true : false);
}

int qsqlist_history::check_non_read(uint account)
{
    int value = -1;
    sqlite3_stmt *stmt;
    QString sql("SELECT SUM(non_read) FROM history_%1;");
    sql = sql.arg(account);

    if(sqlite3_prepare_v2(_db,qtoc(sql),-1,&stmt,nullptr) == SQLITE_OK)
    {
        if(sqlite3_step(stmt) == SQLITE_ROW)
        {
            value = sqlite3_column_int(stmt,0);  //获取结果
        }
    }
    sqlite3_finalize(stmt); //释放预处理语句
    return value;
}

qsqlist_history::name_data qsqlist_history::get_name_data()
{
    return _name;
}
