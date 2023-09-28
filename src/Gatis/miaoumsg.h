#ifndef MIAOUMSG_H
#define MIAOUMSG_H

#include <ArduinoJson.h>

struct DevID {
    String name, type, mac;
};

enum MsgIdFlag {
    DEF = 0b0,
    GET = 0b1,
    PUT = 0b10,
    MAX = 0xFFFFFFFF
};

class MiaouMsg
{
public:
    MiaouMsg();
    MiaouMsg(const DevID &srcIn, const DevID &dstIn, const DynamicJsonDocument &dataIn, const MsgIdFlag &msgIdIn = MsgIdFlag::DEF);
    MiaouMsg(const String &dat);

    DevID src, dst;
    size_t msgID = MsgIdFlag::DEF;
    DynamicJsonDocument *data = nullptr;

    void setData(DynamicJsonDocument docIn);

    void fromJson(DynamicJsonDocument &dataIn);

    DynamicJsonDocument toJson() const;
    String toString() const;
    void setSrc(const DevID &mdev);
    void setDst(const DevID &mdev);
};


#endif // MIAOUMSG_H
