#ifndef TEXTMESSAGE_H
#define TEXTMESSAGE_H

#include <QObject>
#include "database.h"
#include <QTime>
#include "package.h"


class TextMessage : public package
{
public:
    TextMessage();
    virtual void deserialize(QByteArray buffer);
    virtual QByteArray serialize();

    QString getSender();
     QString getReciever();
      QString getMessage();
       QTime gettimeSend();
       SEND_STATE getstateMessage();
private:
    QString sender;
    QString Reciever ;
    QString Message;
    QTime timeSend;
    SEND_STATE stateMessage;
};

#endif // TEXTMESSAGE_H
