#ifndef TEXTMESSAGE_H
#define TEXTMESSAGE_H

#include <QObject>
#include "database.h"
#include <QTime>
#include "Packages.h"

class TextMessage : public DataBase
{
public:
    TextMessage();
private:
    enum SendState
    {
        Sent,
        Sent_Recive,
        Seen,

    };
    HEADERS header;
    QString Message;
    QString Sender;
    QString Receiver;
    QTime timeSend;
    SendState MessageState;
    friend QDataStream &operator<<(QDataStream &out,const TextMessage &data)
    {
        out << static_cast<short>(data.header) << data.Sender << data.Receiver <<
               data.Message << data.timeSend.toString() << static_cast<short>(data.MessageState);
    }
    friend QDataStream &operator>>(QDataStream &in, TextMessage &data)
    {
        QString time;
        short header;
        short MessageState;
        in >> header >> data.Sender >> data.Receiver >>
                data.Message >> time >>  MessageState ;
        data.header = static_cast<HEADERS>(header);
        data.MessageState = static_cast<SendState>(MessageState);
        data.timeSend = data.timeSend.fromString(time);
    }
    };

#endif // TEXTMESSAGE_H
