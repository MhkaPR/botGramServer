#include "textmessage.h"

TextMessage::TextMessage()
{

}


void TextMessage::serialize(QByteArray buffer)
{
    QDataStream in(&buffer,QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_0);
    short headerAsshort,state;
    QString time;
    in >> headerAsshort >> sender >> Message >> Reciever >> time >> state;

    stateMessage = static_cast<SEND_STATE>(state);
    header = static_cast<HEADERS>(headerAsshort);
    timeSend = timeSend.fromString(time);


}

QByteArray TextMessage::deserialize()
{
    QByteArray buffer;

    QDataStream out(&buffer,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << static_cast<short>(header) << sender <<Message << Reciever
           <<timeSend.toString() << static_cast<short>(stateMessage);
    return buffer;
}
