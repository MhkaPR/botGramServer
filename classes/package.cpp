#include "package.h"

package::package()
{

}

QByteArray package::Packeting(HEADERS headerPacket, QByteArray bufferPacket)
{

    QByteArray buf;
    QDataStream out(&buf,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    out << static_cast<short>(headerPacket) << bufferPacket;

    return  buf;


}

package::HEADERS package::unPacketing(QByteArray &bufferPacket)
{

    QDataStream in(&bufferPacket,QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_0);

    QByteArray buf;
    short header;
    in >> header >> buf;
    bufferPacket = buf;
    return static_cast<HEADERS>(header);

}

