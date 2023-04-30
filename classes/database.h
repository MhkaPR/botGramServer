#ifndef DATABASE_H
#define DATABASE_H
#include <QObject>
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

#include <QMessageBox>

struct userInfo
{
    QString username;
    QString password;
    QString email;
};
class DataBase
{
public:
    DataBase();

    int Toint(QString);
    void sendmessage(QString str);
    //connection to dataBase
    //root node must be :dataroot

    bool createDataBase(QString);

    bool save_modifies();
protected:
    QString name;


};

#endif // DATABASE_H
