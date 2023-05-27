#ifndef FILEMESSAGE_H
#define FILEMESSAGE_H

#include <QObject>

#include "package.h"


#include "textmessage.h"

class fileMessage : public package
{
public:
    fileMessage(QString sender);
    virtual void deserialize(QByteArray buffer) override;
    virtual QByteArray serialize() override;

    bool IsEndFile();

    void setroom(QString value);
    QString getroom();


    void setFileName(QString value);
    QString getFileName();


    void setcount_size(QString value);
    QString getcount_size();


    void setData(QByteArray value);
    QByteArray getData();

    void setSender(QString value);
    QString getSender();

    void settimeSend(QDateTime value);
    QDateTime gettimeSend();

    operator TextMessage();


private:
    QString room;
    QString FileName;
    QString count_size;
    QByteArray Data;
    QString Sender;
    QDateTime timeSend;
};

#endif // FILEMESSAGE_H
