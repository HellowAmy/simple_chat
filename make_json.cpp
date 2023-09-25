#include "make_json.h"

make_json::make_json(QObject *parent)
    : QObject(parent)
{

}

bool make_json::send_swap_json(uint ac_to,uint ac_from,QString task,QString content)
{
    bool ret = true;
    try {
        json js = json::parse(cs_sjson::__sjson_swap);
        js["head"] = CS_HEAD;
        js["version"] = CS_VERSION;
        js["target"] = ac_to;
        js["source"] = ac_from;
        js["task"] = task.toStdString();
        js["content"] = content.toStdString();
        emit sn_send_json(js.dump());
    }
    catch(...) { ret = false; }
    return ret;
}
