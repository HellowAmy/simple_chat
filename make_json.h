#ifndef MAKE_JSON_H
#define MAKE_JSON_H

#include <QObject>

#include "file_version.h"
#include "include/nlohmann/json.hpp"

using namespace cs_sjson;
typedef unsigned int uint;
typedef nlohmann::json json;

class make_json : public QObject
{
    Q_OBJECT
public:
    explicit make_json(QObject *parent = nullptr);
    bool send_swap_json(uint ac_to,uint ac_from,QString task,QString content);

signals:
    void sn_send_json(std::string sjson);

private:


};

#endif // MAKE_JSON_H
