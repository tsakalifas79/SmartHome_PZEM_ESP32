#include "miaoumsg.h"

MiaouMsg::MiaouMsg()
{
    src.mac  = "Poutses";
    src.type = "Poutses";
    src.name = "Kaltses";
    dst.mac  = "Mounia";
    dst.type = "Mounia";
    dst.name = "Vrakia";
}

MiaouMsg::MiaouMsg(const String &dat) {
    DynamicJsonDocument msg(dat.length());
    deserializeJson(msg,dat);
    fromJson(msg);
}

MiaouMsg::MiaouMsg(const DevID &srcIn, const DevID &dstIn, const String &dataIn, const MsgIdFlag &msgIdIn) {
    src = std::move(srcIn);
    dst = std::move(dstIn);
    data = std::move(dataIn);
    msgID = msgIdIn;
}

void MiaouMsg::setSrc(const DevID &mdev)
{
    src.type = mdev.type;
    src.name = mdev.name;
    src.mac = mdev.mac;
}

void MiaouMsg::setDst(const DevID &mdev)
{
    dst.type = mdev.type;
    dst.name = mdev.name;
    dst.mac = mdev.mac;
}

void MiaouMsg::fromJson(DynamicJsonDocument &dataIn) {
    DynamicJsonDocument all = dataIn;

    DynamicJsonDocument s = all["SRC"];
    src.type = s["TYPE"].as<String>();
    src.name = s["NAME"].as<String>();
    src.mac  = s["MAC"].as<String>();

    DynamicJsonDocument d = all["DST"];
    dst.type = d["TYPE"].as<String>();
    dst.name = d["NAME"].as<String>();
    dst.mac  = d["MAC"].as<String>();

    msgID = all["MSGID"].as<long>();

    data = all["DATA"].as<String>();
}

DynamicJsonDocument MiaouMsg::toJson() const {

    DynamicJsonDocument all(2048);
    DynamicJsonDocument s(2048);
    s["TYPE"] = src.type;
    s["NAME"] = src.name;
    s["MAC"] = src.mac;
    String srcS;
    serializeJson(s,srcS);
    s.clear();
    DynamicJsonDocument d(2048);
    d["TYPE"] = dst.type;
    d["NAME"] = dst.name;
    d["MAC"] = dst.mac;
    String dstS;
    serializeJson(d,dstS);
    d.clear();

    
    // String dataS;
    // serializeJson(data,dataS);


    all["SRC"] = srcS;
    all["DST"] = dstS;
    all["MSGID"] = String(msgID);
    all["DATA"] = data;

    return all;
}

String MiaouMsg::toString() const {

    DynamicJsonDocument all(2048);
    DynamicJsonDocument s(2048);
    s["TYPE"] = src.type;
    s["NAME"] = src.name;
    s["MAC"] = src.mac;

    DynamicJsonDocument d(2048);
    d["TYPE"] = dst.type;
    d["NAME"] = dst.name;
    d["MAC"] = dst.mac;


    
    // String dataS;
    // serializeJson(data,dataS);

    String srcS = s.as<String>();
    String dstS = d.as<String>();

    all["SRC"] = srcS;
    all["DST"] = dstS;
    all["MSGID"] = String(msgID);
    all["DATA"] = data;



    String out = all.as<String>();
    // serializeJson(all,out);
  
    Serial.println("~~~~~~~~~~~~~~~~~~~TEST OUTPUT~~~~~~~~~~~~~~~~");
    Serial.println(out);

    all.clear();
    s.clear();
    d.clear();
    return out;
}
