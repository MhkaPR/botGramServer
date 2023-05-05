#include "adduser_spacket.h"

AddUser_SPacket::AddUser_SPacket()
{
  header = package::ADDUSER_TO_USERS_DATABASE;
}

void AddUser_SPacket::deserialize(QByteArray buffer)
{
    QDataStream in(&buffer,QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_0);

    short headerMe;

    in >> headerMe >> data;

    header =static_cast<HEADERS>(headerMe);

}

QByteArray AddUser_SPacket::serialize()
{
    QByteArray buf;
    QDataStream out(&buf,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    out << static_cast<short>(header) << data;
    return  buf;
}

QByteArray AddUser_SPacket::getdata()
{
    return data;

}
