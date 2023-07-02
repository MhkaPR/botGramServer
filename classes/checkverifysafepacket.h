#ifndef CHECKVERIFYSAFEPACKET_H
#define CHECKVERIFYSAFEPACKET_H

#include <QObject>

#include "package.h"

class CheckVerifySafePacket : public package
{
public:
    CheckVerifySafePacket();



    virtual void deserialize(QByteArray buffer);
    virtual QByteArray serialize();


    QString getLink();
    QString getAnswer();

    void setLink(QString value);
    void setAnswer(QString value);

private:
    QString Link;
    QString Answer;
};

#endif // CHECKVERIFYSAFEPACKET_H
