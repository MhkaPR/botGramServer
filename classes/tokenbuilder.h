#ifndef TOKENBUILDER_H
#define TOKENBUILDER_H

#include <QObject>
#include "database.h"
#include <QSqlDatabase>
#include <QRegularExpression>
#include <QRandomGenerator>


class TokenBuilder : DataBase
{
public:
    TokenBuilder(QString Db_name,QSqlDatabase Mydb);
     QString token();

private:
    const QString TableName= "Users_Information";
    QString databaseName;
    QSqlDatabase db;
    QString Token;


    bool IsCorrectToken(QString str);
};


#endif // TOKENBUILDER_H
