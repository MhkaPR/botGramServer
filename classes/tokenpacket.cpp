#include "tokenpacket.h"

TokenPacket::TokenPacket()
{
    header = package::TOKENUSER;

}

QString TokenPacket::getToken()
{
    return  Token;

}

void TokenPacket::deserialize(QByteArray buffer)
{
    QDataStream in(&buffer,QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_0);
    qint64 headerAsshort;
    in >> headerAsshort >> Token;
    header = static_cast<HEADERS>(headerAsshort);

}

QByteArray TokenPacket::serialize()
{
    QByteArray buf;
    QDataStream out(&buf,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    out << static_cast<short>(header) << Token;
    return  buf;

}

void TokenPacket::setToken(QString value)
{
    Token = value;

}
