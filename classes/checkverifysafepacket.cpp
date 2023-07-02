#include "checkverifysafepacket.h"

CheckVerifySafePacket::CheckVerifySafePacket()
{
        header = package::SAFE_VERIFY;
}

void CheckVerifySafePacket::deserialize(QByteArray buffer)
{

    QDataStream in(&buffer,QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_0);

    qint64 headerMe;

    in >> headerMe >> Link >> Answer;

    header =static_cast<HEADERS>(headerMe);
}

QByteArray CheckVerifySafePacket::serialize()
{
    QByteArray buf;
    QDataStream out(&buf,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    out << static_cast<short>(header) << Link << Answer;
    return  buf;

}

QString CheckVerifySafePacket::getLink()
{
    return  Link;

}

QString CheckVerifySafePacket::getAnswer()
{
    return  Answer;

}

void CheckVerifySafePacket::setLink(QString value)
{
    Link = value;
}

void CheckVerifySafePacket::setAnswer(QString value)
{
    Answer = value;
}
