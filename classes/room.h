#ifndef ROOM_H
#define ROOM_H

#include <QObject>
#include "client.h"
#include "textmessage.h"

class Room
{
public:
    Room();

    virtual void Receive()= 0;
    virtual void Send()= 0;

    static Room *selectRoom(short RoomID);

protected:

    QVector<client> clients;
    QVector<TextMessage> Messages;

private:


};

#endif // ROOM_H
