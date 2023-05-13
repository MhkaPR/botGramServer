#include "loginpacket.h"

loginPacket::loginPacket()
{
    header = package::VERIFY;

}

QByteArray loginPacket::getJsonLoginData()
{
    return JsonLoginData;

}

void loginPacket::setJsonLoginData(QByteArray value)
{
    JsonLoginData = value;
}

void loginPacket::deserialize(QByteArray buffer)
{
    QDataStream in(&buffer,QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_0);

    qint64 headerMe;

    in >> headerMe >> JsonLoginData;

    header =static_cast<HEADERS>(headerMe);
}

QByteArray loginPacket::serialize()
{
    QByteArray buf;
    QDataStream out(&buf,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    out << static_cast<short>(header) << JsonLoginData;
    return  buf;
}
