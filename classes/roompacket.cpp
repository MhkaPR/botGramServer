#include "roompacket.h"

RoomPacket::RoomPacket()
{

}

void RoomPacket::deserialize(QByteArray buffer)
{

    QDataStream in(&buffer,QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_0);

    short headerMe;
    short Roomidasshort;

    in >> headerMe >> Roomidasshort >> nextData;

    header = static_cast<HEADERS>(headerMe);
    RoomID = static_cast<RoomType>(Roomidasshort);

}

QByteArray RoomPacket::serialize()
{
    QByteArray buf;
    QDataStream out(&buf,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    out << static_cast<short>(header) << static_cast<short>(RoomID) << nextData;
    return  buf;

}
