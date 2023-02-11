#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "miaoumsg.h"

class InnerWorld {
public:
    virtual void take(const MiaouMsg &msg)=0;
};

class OutterWorld {
protected:
    InnerWorld *mDev = nullptr;
public:
    OutterWorld(InnerWorld *dev = nullptr) {
        mDev = dev;
    }
    virtual void give(const MiaouMsg &msg)=0;
    void setDev(InnerWorld *newDev);
};

inline void OutterWorld::setDev(InnerWorld *newDev)
{
    mDev = newDev;
}
#endif // CALLBACKS_H
