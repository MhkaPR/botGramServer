#ifndef LOGINPACKET_H
#define LOGINPACKET_H

#include <QObject>

#include "package.h"

class loginPacket : public package
{
public:
    loginPacket();

    QByteArray getJsonLoginData();
    void setJsonLoginData(QByteArray value);

    virtual void deserialize(QByteArray buffer) override;
    virtual QByteArray serialize() override;

private:
    QByteArray JsonLoginData;
};

#endif // LOGINPACKET_H
