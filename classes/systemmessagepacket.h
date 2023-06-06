#ifndef SYSTEMMESSAGEPACKET_H
#define SYSTEMMESSAGEPACKET_H

#include <QObject>

#include "package.h"

class systemMessagePacket : public package
{
public:
    systemMessagePacket();



    virtual void deserialize(QByteArray buffer) override;
    virtual QByteArray serialize() override;

    SysCodes getSysmsg();
    void setSysmsg(SysCodes value);
    QByteArray getInformation();

private:
    SysCodes Sysmsg;
    QByteArray information;
};

#endif // SYSTEMMESSAGEPACKET_H
