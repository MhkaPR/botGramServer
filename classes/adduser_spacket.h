#ifndef ADDUSER_SPACKET_H
#define ADDUSER_SPACKET_H

#include <QObject>

#include "package.h"

class AddUser_SPacket : public package
{
public:
    AddUser_SPacket();


    virtual void deserialize(QByteArray buffer);
    virtual QByteArray serialize();

    QByteArray getdata();
private:
 QByteArray data;
};

#endif // ADDUSER_SPACKET_H
