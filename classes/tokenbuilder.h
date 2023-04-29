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


    bool IsCorrectToken(QString str)
    {
        QSqlQuery Q(db);

        Q.prepare("SELECT token FROM "+TableName+" WHERE token = :t");
        Q.bindValue(":t",str);
        if(!Q.exec())
        {
            sendmessage(Q.lastError().text());
            Q.clear();
            db.close();
            exit(1);
        }
        if(Q.next()) return false;
        return true;
    }
};


#endif // TOKENBUILDER_H
