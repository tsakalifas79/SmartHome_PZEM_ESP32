#include "whiskvice.h"

const DevID &WhiskVice::getSelf() const
{
    return self;
}

WhiskVice::WhiskVice(const String &name, const String &type, const String &mac) {
    setSelf(name, type, mac);
}

WhiskVice::WhiskVice()
{

}

void WhiskVice::setSelf(const String &name, const String &type, const String &mac) {
    self.name = name;
    self.type = type;
    self.mac  = mac;
}

void WhiskVice::take(const MiaouMsg &msg) {

    // qDebug() << "WhiskVice::take - before" << self.name << others.size();
    if(others.count(msg.src.mac.c_str()) == 0)
    {
        others.insert( { msg.src.mac.c_str(), msg.src } );
    }

    // qDebug() << "WhiskVice::take" << self.name << msg.toJson();
    // qDebug() << "WhiskVice::take - after" << self.name << others.size();

}
