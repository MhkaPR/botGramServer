#ifndef TOKENPACKET_H
#define TOKENPACKET_H

#include <QObject>

#include "package.h"

class TokenPacket : public package
{
public:
    TokenPacket();

    QString getToken();

    virtual void deserialize(QByteArray buffer);
    virtual QByteArray serialize();

    void setToken(QString value);

private:

     QString Token;
};

#endif // TOKENPACKET_H
