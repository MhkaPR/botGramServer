#ifndef CLIENT_MSSAGES_H
#define CLIENT_MSSAGES_H

#include <QObject>
#include "server.h"
#include "database.h"

//database
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQueryModel>

class Client_Mssages : DataBase
{
public:
    enum Errors
    {
        USER_NOT_FOUND_WITH_THIS_TOKEN,
        USER_FOUND_OK,
        DATABASE_ERROR

    };
    Client_Mssages(TextMessage message);

    short MessageConfrime(QSqlDatabase Db,QString tableName = "Users_Information");
    static short ConnectConfrime(QSqlDatabase Db,QString& Token_username,QString tableName = "Users_Information");



private:
    QSqlDatabase db;
    QString TableName;
    TextMessage MessageStruct;

};


#endif // CLIENT_MSSAGES_H
