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
    virtual void serialize(QByteArray buffer);
    virtual QByteArray deserialize();
private:


    QString sender;
    QString Reciever;
    QString Message;
    QTime timeSend;
    SEND_STATE stateMessage;
};

#endif // TEXTMESSAGE_H
