#include "client_mssages.h"

Client_Mssages::Client_Mssages(TextMessage message) : MessageStruct(message)
{

}

short Client_Mssages::MessageConfrime(QSqlDatabase Db, QString tableName)
{
    db = Db;//to private
    TableName =tableName;//to private
    QSqlQuery queryCheckMessage(db);

    queryCheckMessage.prepare("SELECT Token FROM "+tableName+" WHERE Token = :t");
    queryCheckMessage.bindValue(":t",MessageStruct.getSender());
    if(!queryCheckMessage.exec())
    {
        sendmessage(queryCheckMessage.lastError().text());
        queryCheckMessage.clear();
        return DATABASE_ERROR;
    }
    if(!queryCheckMessage.next()) return USER_NOT_FOUND_WITH_THIS_TOKEN;
    return USER_FOUND_OK;
}

short Client_Mssages::ConnectConfrime(QSqlDatabase Db, QString& Token_username, QString tableName)
{


    QSqlQuery queryCheckMessage(Db);

    queryCheckMessage.prepare("SELECT * FROM "+tableName+" WHERE Token = :t");
    queryCheckMessage.bindValue(":t",Token_username);
    if(!queryCheckMessage.exec())
    {
        QMessageBox *m= new QMessageBox();
        m->setText(queryCheckMessage.lastError().text());
        m->exec();
        delete m;

        queryCheckMessage.clear();
        return DATABASE_ERROR;
    }
    if(!queryCheckMessage.next()) return USER_NOT_FOUND_WITH_THIS_TOKEN;

    Token_username = queryCheckMessage.value("username").toString();
    return USER_FOUND_OK;
}

short Client_Mssages::add_in_Room(QString RoomName,QString sender,QString Date,QString message)
{
    QSqlQuery query_ADD_Message_in_Room(db);

    query_ADD_Message_in_Room.prepare("INSERT INTO "+RoomName+
                    " (name,message,date)"
                    " VALUES (:n,:m,:d)");
    query_ADD_Message_in_Room.bindValue(":n",sender);
    query_ADD_Message_in_Room.bindValue(":m",message);
    query_ADD_Message_in_Room.bindValue(":d",Date);
    if(!query_ADD_Message_in_Room.exec())
    {
        QMessageBox *m= new QMessageBox();
        m->setText(query_ADD_Message_in_Room.lastError().text());
        m->exec();
        delete m;

        query_ADD_Message_in_Room.clear();
        return DATABASE_ERROR;

    }
    return MESSAGE_SUCCESSFULLY_ADDED;
}

short Client_Mssages::update_last_update(QString username,QString RoomName, QString date)
{


    QString Info = username + ":"+date;

    QSqlQuery query_update_last_update(db);

    query_update_last_update.prepare("UPDATE "+
                                     username+" SET lastMessage_Info = :data WHERE Rooms = :room");

    query_update_last_update.bindValue(":data",Info);
    query_update_last_update.bindValue(":room",RoomName);


    if(!query_update_last_update.exec())
    {
        QMessageBox *m= new QMessageBox();
        m->setText(query_update_last_update.lastError().text());
        m->exec();
        delete m;

        query_update_last_update.clear();
        return DATABASE_ERROR;
    }

    return UPDATE_LAST_DATE_MESSAGE_SUCCESSFULLY;
}
