#ifndef WHISKVICE_H
#define WHISKVICE_H

#include "callbacks.h"

#include <set>
#include <unordered_map>

#ifndef CATS_PORT
#define CATS_PORT 6869
#endif

#ifndef MULTI_GROUP_ADDR
#define MULTI_GROUP_ADDR "192.168.2.199"
#endif

class WhiskVice: public InnerWorld
{
    DevID self;
    std::unordered_map<std::string, DevID> others;

public:
    WhiskVice(const String &name, const String &type, const String &mac);
    WhiskVice();


    void setSelf(const String &name, const String &type, const String &mac);
    virtual void take(const MiaouMsg &msg);

    const DevID &getSelf() const;
};

#endif // WHISKVICE_H
