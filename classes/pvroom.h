#ifndef PVROOM_H
#define PVROOM_H

#include <QObject>
#include "room.h"

class pvRoom : public Room
{
public:
    pvRoom();

    virtual void Recieve();
    virtual void Send();


private:

};

#endif // PVROOM_H
