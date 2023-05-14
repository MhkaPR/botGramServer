#ifndef TEXTMESSAGE_H
#define TEXTMESSAGE_H

#include <QObject>
#include "database.h"
#include <QDateTime>
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
       QDateTime gettimeSend();
       SEND_STATE getstateMessage();

       void setSender(QString value);
private:
    QString sender;
    QString Reciever ;
    QString Message;
    QDateTime timeSend;
    SEND_STATE stateMessage;
};

#endif // TEXTMESSAGE_H
