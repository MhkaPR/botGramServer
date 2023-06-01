#ifndef UPDATEMESSAGEPACKET_H
#define UPDATEMESSAGEPACKET_H

#include <QObject>
#include "client_mssages.h"
#include "textmessage.h"

class updateMessagePacket
{


public:
    updateMessagePacket(TextMessage message);
    void getRoomsData();



private:

    TextMessage msg;


};

#endif // UPDATEMESSAGEPACKET_H
