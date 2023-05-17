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


#include "textmessage.h"
#include "queriespacket.h"
#include <QMap>

class Client_Mssages : DataBase
{
public:
    enum Errors
    {
        USER_NOT_FOUND_WITH_THIS_TOKEN,
        USER_FOUND_OK,
        DATABASE_ERROR,
        MESSAGE_SUCCESSFULLY_ADDED,
        UPDATE_LAST_DATE_MESSAGE_SUCCESSFULLY,
        ADD_DATA_SUCCESSFULLY

    };



    Client_Mssages(TextMessage message);

    short MessageConfrime(QSqlDatabase Db,QString tableName = "Users_Information");
    static short ConnectConfrime(QSqlDatabase Db,QString& Token_username,QString tableName = "Users_Information");
    short add_in_Room(QString* RoomName,QString sender,QString Date,QString message);
    short update_last_update(QString username,QString sender_update,QString RoomName, QString date);
    QString get_LastUpdate(QString username,QString RoomName);
    QStringList sendForRoomClients(QMap<QString,QTcpSocket*>& clients,QString lastupdate,TextMessage msg,QString tableName="Rooms");
    static QString getUsername(QString Token,QSqlDatabase Db, QString tableName="Users_Information");

    template<class T>
    short addDataInColumn(T data,QString ColName,QString tableName = "Rooms");


    short addData_inPersonalTable(QString username,QString RoomName);



private:

    QString TableName;
    TextMessage MessageStruct;

    bool IsUpdateData(QString SenderName, QString RoomName,QString lastSenderUpdate);
    QByteArray getupdates(QString lastUserUpdate,TextMessage msg);

};


#endif // CLIENT_MSSAGES_H
