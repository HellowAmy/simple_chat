#ifndef SQLITE_READ_HISTORY_H
#define SQLITE_READ_HISTORY_H

#include "sqlite_op.h"
#include "typedef_struct.h"
#include "Tvlog.h"

using namespace typedef_struct;

//!
//! 类说明：读取数据库消息，自动标记为已读
//!
class sqlite_read_history : public QObject
{
public:
    explicit sqlite_read_history(QObject *parent = nullptr)
        : QObject(parent) {}

    void init_db(sqlite_history *db)
    {
        _db = db;
    }

    void read_history(int64 account,vector<ct_msg_type> &vec_msg)
    {
        //读取历史记录
        vector<tuple<int64, int64, int64, string, string, string>> vec_line;
        if(_db->select_history(account,vec_line))
        {
            //显示信息到屏幕
            for(auto a:vec_line)
            {
                ct_msg_type ct{std::get<0>(a),std::get<3>(a),std::get<4>(a),std::get<5>(a)};
                vec_msg.push_back(ct);

                //去除未读
                if(_db->update_non_read(account,ct.time,0) == false)
                { vlogw("update_non_read failed"); }
            }
        }
        else vlogw("read history failed" $(account));
    }

    void read_history_non_read(int64 account,vector<ct_msg_type> &vec_msg)
    {
        //读取历史记录
        vector<tuple<int64, int64, int64, string, string, string>> vec_line;
        if(_db->select_non_read(account,vec_line))
        {
            //显示信息到屏幕
            for(auto a:vec_line)
            {
                ct_msg_type ct{std::get<0>(a),std::get<3>(a),std::get<4>(a),std::get<5>(a)};
                vec_msg.push_back(ct);

                //去除未读
                if(_db->update_non_read(account,ct.time,0) == false)
                { vlogw("update_non_read failed"); }
            }
        }
        else vlogw("read history failed" $(account));
    }

protected:
    sqlite_history *_db = nullptr;
};

#endif // SQLITE_READ_HISTORY_H
