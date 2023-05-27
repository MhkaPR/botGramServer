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

    bool getIsFile();
    QString getSender();
    QString getReciever();
    QString getMessage();
    QDateTime gettimeSend();
    QByteArray getbuf();
    SEND_STATE getstateMessage();

    void setSender(QString value);
    void setReceiver(QString value);
    void setbuf(QByteArray value);
    void setmessage(QString value);
    void setIsFile(bool value);

private:
    bool IsFile=false;
    QString sender;
    QString Reciever ;
    QString Message;
    QDateTime timeSend;
    SEND_STATE stateMessage;
    QByteArray buf;

};

#endif // TEXTMESSAGE_H
