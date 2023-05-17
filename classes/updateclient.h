#ifndef UPDATECLIENT_H
#define UPDATECLIENT_H

#include <QObject>

#include "database.h"
#include "package.h"
class updateClient : public DataBase,public package
{
public:
    virtual void deserialize(QByteArray buffer);
    virtual QByteArray serialize();

    updateClient();

   QJsonDocument getDocJson();
   void setDocJson(QJsonDocument doc);

private:
 QJsonDocument docJson;
};

#endif // UPDATECLIENT_H
