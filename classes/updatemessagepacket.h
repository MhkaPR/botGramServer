#ifndef UPDATEMESSAGEPACKET_H
#define UPDATEMESSAGEPACKET_H

#include <QObject>
#include "client_mssages.h"
#include "textmessage.h"
#include "database.h"

class updateMessagePacket: public DataBase ,public package
{
public:
    updateMessagePacket();
    void receiveRoomsData(QString sender);

    void deserialize(QByteArray buffer) override;
    QByteArray serialize() override;


    QByteArray buffer;

private:

};

#endif // UPDATEMESSAGEPACKET_H
