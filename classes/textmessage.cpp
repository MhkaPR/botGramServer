#include "textmessage.h"

TextMessage::TextMessage()
{
    header = package::TEXTMESSAGE;

}


void TextMessage::deserialize(QByteArray buffer)
{
    QDataStream in(&buffer,QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_0);
    qint64 headerAsshort;
      short state;
    QString time;
    in >> headerAsshort >> sender  >>Reciever>> Message >> time >> state;

    stateMessage = static_cast<SEND_STATE>(state);
    header = static_cast<HEADERS>(headerAsshort);
    timeSend = timeSend.fromString(time);


}

QByteArray TextMessage::serialize()
{
    QByteArray buffer;

    QDataStream out(&buffer,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << static_cast<short>(header) << sender << Reciever << Message
        <<timeSend.toString() << static_cast<short>(stateMessage);
    return buffer;
}

bool TextMessage::getIsFile()
{
    return IsFile;
}

QString TextMessage::getSender()
{
    return  sender;
}

QString TextMessage::getReciever()
{
    return Reciever;
}

QString TextMessage::getMessage()
{
    return Message;
}

QDateTime TextMessage::gettimeSend()
{
    return timeSend;
}

QByteArray TextMessage::getbuf()
{
    return buf;
}

package::SEND_STATE TextMessage::getstateMessage()
{
    return stateMessage;
}

void TextMessage::setSender(QString value)
{
    sender = value;
}

void TextMessage::setReceiver(QString value)
{
    Reciever = value;
}

void TextMessage::setbuf(QByteArray value)
{
    buf = value;
}

void TextMessage::setmessage(QString value)
{
    Message = value;
}

void TextMessage::setIsFile(bool value)
{
    IsFile =value;
}


