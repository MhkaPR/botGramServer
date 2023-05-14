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
        m->setText("add in room:"+query_ADD_Message_in_Room.lastError().text());
        m->exec();
        delete m;

        query_ADD_Message_in_Room.clear();
        return DATABASE_ERROR;

    }
    return MESSAGE_SUCCESSFULLY_ADDED;
}

short Client_Mssages::update_last_update(QString username,QString sender_update,QString RoomName, QString date)
{



    QSqlQuery query_update_last_update(db);

    query_update_last_update.prepare("UPDATE "+
                                     username+" SET lastMessage_Info = :data , updateSender = :up WHERE Rooms = :room");

    query_update_last_update.bindValue(":data",date);
     query_update_last_update.bindValue(":up",sender_update);
    query_update_last_update.bindValue(":room",RoomName);


    if(!query_update_last_update.exec())
    {
        QMessageBox *m= new QMessageBox();
        m->setText("update_last_update: "+query_update_last_update.lastError().text());
        m->exec();
        delete m;

        query_update_last_update.clear();
        return DATABASE_ERROR;
    }

    return UPDATE_LAST_DATE_MESSAGE_SUCCESSFULLY;
}



void Client_Mssages::sendForRoomClients(QMap<QString,QTcpSocket*>& clients,QString lastupdate,TextMessage msg,QString tableName)
{


    QSqlQuery query_SendRommClients(db);

    query_SendRommClients.prepare("SELECT "+
                                  msg.getReciever()+" FROM "+tableName+" WHERE "+msg.getReciever()+" != :sender");

    query_SendRommClients.bindValue(":sender",msg.getSender());

    if(!query_SendRommClients.exec())
    {
        QMessageBox *m= new QMessageBox();
        m->setText(query_SendRommClients.lastError().text());
        m->exec();
        delete m;

        query_SendRommClients.clear();
        //return DATABASE_ERROR;
    }



    while (query_SendRommClients.next()) {

     QString recieverName = query_SendRommClients.value(msg.getReciever()).toString();
    if(clients.contains(recieverName))
    {
        if(!IsUpdateData(recieverName,msg.getReciever(),lastupdate))
        {
            QByteArray buf = getupdates(lastupdate,msg);
            clients[recieverName]->write(buf);
            clients[recieverName]->waitForBytesWritten();
            update_last_update(recieverName,msg.getSender(),msg.getReciever(),msg.gettimeSend().toString());

        }

    }




    }

    // return UPDATE_LAST_DATE_MESSAGE_SUCCESSFULLY;

}

QString Client_Mssages::getUsername(QString Token,QSqlDatabase Db , QString tableName)
{

    QSqlQuery query_getusername(Db);

    query_getusername.prepare("SELECT username FROM "+tableName+" WHERE Token "+" == :u");

    query_getusername.bindValue(":u",Token);

    if(!query_getusername.exec())
    {
        QMessageBox *m= new QMessageBox();
        m->setText(query_getusername.lastError().text());
        m->exec();
        delete m;

        query_getusername.clear();
        //return DATABASE_ERROR;
    }
    if(query_getusername.next())
    {
        return  query_getusername.value("username").toString();
    }
    return "";

}
QString Client_Mssages::get_LastUpdate(QString username, QString RoomName)
{
    QSqlQuery query_getLastUpdate(db);

    query_getLastUpdate.prepare("SELECT lastMessage_Info FROM "+username+" WHERE Rooms = :roomname");

    query_getLastUpdate.bindValue(":roomname",RoomName);

    if(!query_getLastUpdate.exec())
    {
        QMessageBox *m= new QMessageBox();
        m->setText(query_getLastUpdate.lastError().text());
        m->exec();
        delete m;

        query_getLastUpdate.clear();
        exit(1);

    }

    if(query_getLastUpdate.next())
    {
        return  query_getLastUpdate.value("lastMessage_Info").toString();
    }


}
bool Client_Mssages::IsUpdateData(QString SenderName, QString RoomName,QString lastSenderUpdate)
{
    QSqlQuery query_check_last_update(db);

    query_check_last_update.prepare("SELECT lastMessage_Info FROM "+SenderName+" WHERE Rooms = :roomname");

    query_check_last_update.bindValue(":roomname",RoomName);


    if(!query_check_last_update.exec())
    {
        QMessageBox *m= new QMessageBox();
        m->setText(query_check_last_update.lastError().text());
        m->exec();
        delete m;

        query_check_last_update.clear();
        exit(1);

    }

    if(query_check_last_update.next())
    {
       if(query_check_last_update.value("lastMessage_Info").isNull())
       {
           return  false;
       }
       else if(query_check_last_update.value("lastMessage_Info").toString() == lastSenderUpdate)
       {
           return true;
       }
       else
       {
           return false;
       }
    }
    return false;
}

QByteArray Client_Mssages::getupdates( QString lastSenderUpdate, TextMessage msg)
{
    QSqlQuery query_getUpdates(db);

    query_getUpdates.prepare("SELECT * FROM "+msg.getReciever()+" WHERE date > :d "
                                                                "ORDER BY date DESC");

    query_getUpdates.bindValue(":d",lastSenderUpdate);


    if(!query_getUpdates.exec())
    {
        QMessageBox *m= new QMessageBox();
        m->setText(query_getUpdates.lastError().text());
        m->exec();
        delete m;

        query_getUpdates.clear();
        exit(1);
    }
    QJsonObject objs;
    while(query_getUpdates.next())
    {
        QJsonArray data;
        QString messageNew = query_getUpdates.value("message").toString();
        data.append(messageNew);
        QString timeNew = query_getUpdates.value("date").toString();
        data.append(timeNew);
        objs.insert(msg.getSender(),data);
    }
    QJsonDocument docMessages;
    docMessages.setObject(objs);

    return  docMessages.toJson();
}
