#ifndef SEARCHUSER_H
#define SEARCHUSER_H


//database
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQueryModel>

//json
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


#include <QObject>
#include "package.h"
#include <QMessageBox>


class searchUserPackat : public package
{
public:

    enum searchErrors : short
    {
        SUCCESSFULYFINDED,
        NOTFOUND,
        DATABASEERROR,
    };
    searchUserPackat();
    void deserialize(QByteArray buffer) override;
    QByteArray serialize() override;

    void setemail(QString value);
    QString getemail();


    void setuserdata(QStringList value);
    QStringList getuserdata();

    short findUser(QSqlDatabase db,QString tableName = "Users_Information");



private:
    //QString sender;
    QString email;
   QString username;
   QString name;
};

#endif // SEARCHUSER_H
