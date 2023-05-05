#include "systemmessagepacket.h"

systemMessagePacket::systemMessagePacket()
{

    header = package::SYSTEM;

}



void systemMessagePacket::deserialize(QByteArray buffer)
{
    QDataStream in(&buffer,QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_0);

    short headerMe;
    short Sys;

    in >> headerMe >> Sys;

    header =static_cast<HEADERS>(headerMe);
    Sysmsg =static_cast<SysCodes>(Sys);
}

QByteArray systemMessagePacket::serialize()
{
    QByteArray buf;
    QDataStream out(&buf,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    out << static_cast<short>(header) << static_cast<short>(Sysmsg);
    return  buf;
}

package::SysCodes systemMessagePacket::getSysmsg()
{
    return Sysmsg;

}

void systemMessagePacket::setSysmsg(package::SysCodes value)
{
    Sysmsg = value;

}
