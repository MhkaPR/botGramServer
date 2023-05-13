#ifndef CONNECTVERIFY_H
#define CONNECTVERIFY_H

#include <QObject>

#include "package.h"
#include "client_mssages.h"

class connectVerify : public package
{
public:
    connectVerify();

    virtual void deserialize(QByteArray buffer) override;
    virtual QByteArray serialize() override;


    QString getToken();
    QString Token;
    QJsonDocument doc;

private:

};

#endif // CONNECTVERIFY_H
