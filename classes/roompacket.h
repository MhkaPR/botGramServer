#ifndef ROOMPACKET_H
#define ROOMPACKET_H

#include <QObject>
#include "package.h"

class RoomPacket : public package
{
public:
    RoomPacket();


    virtual void deserialize(QByteArray buffer) override;
    virtual QByteArray serialize() override;


    RoomType getRoomID();
    void setRoomID(RoomType value);

    QByteArray getnextData();
    void setnextData(QByteArray value);

private:
    RoomType RoomID;
    QByteArray nextData;

};

#endif // ROOMPACKET_H
