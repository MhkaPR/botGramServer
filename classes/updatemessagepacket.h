#ifndef UPDATEMESSAGEPACKET_H
#define UPDATEMESSAGEPACKET_H

#include <QObject>
#include "client_mssages.h"
#include "textmessage.h"
#include "database.h"

class updateMessagePacket: public DataBase
{
public:
    updateMessagePacket(TextMessage message);
    void getRoomsData();



private:
    TextMessage msg;
};

#endif // UPDATEMESSAGEPACKET_H
