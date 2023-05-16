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
    enum Errors
    {
        USER_NOT_FOUND_WITH_THIS_TOKEN,
        USER_FOUND_OK,
        DATABASE_ERROR,
        MESSAGE_SUCCESSFULLY_ADDED,
        UPDATE_LAST_DATE_MESSAGE_SUCCESSFULLY,
        COLUMN_SUCCESSFULLY_ADDED,
        ADD_DATA_SUCCESSFULLY,
        ADDED_TABLE

    };
    DataBase();

    int Toint(QString);
    void sendmessage(QString str);

    //connection to dataBase
    //root node must be :dataroot



    bool createDataBase(QString);
    bool IsColumnInTable(QString Colname, QString tableName = "Rooms");
    short add_columnInTable(QString ColName , QString tableName = "Rooms");

    short create_Table(QString tableName,const QMap<QString,QString> ColInfo);


    bool save_modifies();
protected:
    QString name;
    QSqlDatabase db;


};

#endif // DATABASE_H
