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
class DataBase
{
public:
    DataBase();
    void sendMessage(QString);
    int Toint(QString);

    //connection to dataBase
    //root node must be :dataroot

    bool createDataBase(QString);

    bool save_modifies();

};

#endif // DATABASE_H
