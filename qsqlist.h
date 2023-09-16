#ifndef QSQLIST_H
#define QSQLIST_H

#include <QObject>
#include <QVariant>
#include <tuple>

#include "include/sqlist3/sqlite3.h"
//#include "third_party/sqlite3/sqlite3.h"

using std::tuple;
using std::string;
typedef unsigned int uint;

//!
//! 类说明： 数据库基类，提供通用操作
//!
class qsqlist_base : public QObject
{
    Q_OBJECT
public:
    explicit qsqlist_base(QObject *parent = nullptr);
    ~qsqlist_base();

    //通用操作
    bool open_db(QString file);      //打开
    bool close_db();                 //关闭
    bool delete_db(QString table);   //删除数据
    bool drop_db(QString table);     //删除表
    bool select_db(QString table,QVector<QString> &data);   //查数据

    //特例操作
    int create_test(QString table);
    int insert_test(QString table,const tuple<int,string> &data);
    int select_test(QString table,QVector<tuple<int,string>> &data);

    QString get_error_exec();   //错误显示
    QString get_error();        //错误显示

protected:
    sqlite3 *_db = nullptr;
    char* _error = nullptr;
};

//!
//! 类说明： 聊天历史数据库记录，重写操作（打开，创建，插入，查未读数）
//!
class qsqlist_history : public qsqlist_base
{
    Q_OBJECT
public:
    struct name_data
    {
        QString send_time;
        QString feed_back;
        QString non_read;
        QString types;
        QString object;
        QString content;
    };
public:
    explicit qsqlist_history(QObject *parent = nullptr);

    bool open_history(uint account);
    bool create_history(uint account);
    bool insert_history(uint account,const tuple<uint,uint,uint,uint,uint,string> &data);
    bool update_history(uint account,uint id,QString name,QVariant value);
    int check_non_read(uint account);

    name_data get_name_data();

private:
    QString _name_history = "../data/history_%1.db";
    name_data _name;
};

#endif // QSQLIST_H
