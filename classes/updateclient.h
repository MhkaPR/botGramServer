#ifndef UPDATECLIENT_H
#define UPDATECLIENT_H

#include <QObject>

#include "database.h"
#include "package.h"
class updateClient :public package
{
public:
    virtual void deserialize(QByteArray buffer);
    virtual QByteArray serialize();

    updateClient(QSqlDatabase DB);

    QByteArray getDocJson();
    void setDocJson(QByteArray doc);
    void fixUpdates(QString username);

    int IsApply = false;

private:

    QSqlDatabase db;
    QByteArray docJsonByteArray;

};

#endif // UPDATECLIENT_H
