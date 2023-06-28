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

short Client_Mssages::add_in_Room(QString* RoomName,QString sender,QString Date,QString message,bool Isfile)
{

    QDir checkdirection;


    QString cur = QDir::currentPath()+"/files/"+(*RoomName);

    checkdirection.setPath(cur);
    if(!checkdirection.exists())
        QDir().mkpath(cur);
    QStringList RoomData = RoomName->split("_");

    //check room exist.
    if(!IsColumnInTable(*RoomName))
    {
        // add a culomn in Rooms table
        short err = add_columnInTable(*RoomName);
        if(err == DataBase::COLUMN_SUCCESSFULLY_ADDED)
        {
            // add at least of members in in that culomn



            err = addDataInColumn(RoomData,*RoomName);
            if(err== Client_Mssages::ADD_DATA_SUCCESSFULLY)
            {

                // create a table from that culomn that it's name is ex: A
                QMap<QString,QString> data;
                data.insert("name","text");
                data.insert("message","text");
                data.insert("date","text");
                data.insert("isFile","integer");
                err = create_Table(*RoomName,data);
                if(err == DataBase::ADDED_TABLE)
                {
                    // add a row in tables of members that are in the Room table
                    // write in the cell of Rooms culomn of member's table : A -> name of new Room
                    err = addData_inPersonalTable(sender,*RoomName);
                    if(err == Client_Mssages::ADD_DATA_SUCCESSFULLY)
                    {
                        QStringList users = RoomData;
                        if(users[0]=="pv")
                        {
                            users.removeAt(0);
                            foreach (QString var, users)
                            {
                                if(var != sender)
                                {


                                    err = addData_inPersonalTable(var,*RoomName);
                                }
                                if(err != Client_Mssages::ADD_DATA_SUCCESSFULLY)
                                {
                                    qDebug() << "inside Error@@@";
                                    //handle error
                                    exit(1);
                                }
                            }
                        }

                    }else {
                        //hanlde error
                         qDebug() << "OutSide Error@@@";
                        exit(1);
                    }





                }
                else {
                    //handle errors
                    exit(1);
                }


            }
            else
            {
                //handle errors
                exit(1);
            }



        }
        else
        {
            //handle error
            exit(1);
        }
    }





    QSqlQuery query_ADD_Message_in_Room(db);

    query_ADD_Message_in_Room.prepare("INSERT INTO "+(*RoomName)+
                                      " (name,message,date,isFile)"
                                      " VALUES (:n,:m,:d,"+QString::number(Isfile)+")");
    query_ADD_Message_in_Room.bindValue(":n",sender);
    query_ADD_Message_in_Room.bindValue(":m",message);
    query_ADD_Message_in_Room.bindValue(":d",Date);

    if(!query_ADD_Message_in_Room.exec())
    {
        if(RoomData[0]=="pv")
        {
            RoomData.swap(1,2);

            *RoomName = RoomData.join("_");


            query_ADD_Message_in_Room.clear();
            query_ADD_Message_in_Room.prepare("INSERT INTO "+(*RoomName)+
                                              " (name,message,date,isFile)"
                                              " VALUES (:n,:m,:d,"+QString::number(Isfile)+")");
            query_ADD_Message_in_Room.bindValue(":n",sender);
            query_ADD_Message_in_Room.bindValue(":m",message);
            query_ADD_Message_in_Room.bindValue(":d",Date);
            if(!query_ADD_Message_in_Room.exec())
            {
                QMessageBox *m= new QMessageBox();
                m->setText("add in room -> "
                           "swaped data:"+query_ADD_Message_in_Room.lastError().text());
                m->exec();
                delete m;


                query_ADD_Message_in_Room.finish();
                return DATABASE_ERROR;
            }

        }
        else
        {
            QMessageBox *m= new QMessageBox();
            m->setText("add in room:"+query_ADD_Message_in_Room.lastError().text());
            m->exec();
            delete m;


            query_ADD_Message_in_Room.clear();
            return DATABASE_ERROR;
        }

    }
    query_ADD_Message_in_Room.finish();
    return MESSAGE_SUCCESSFULLY_ADDED;
}

short Client_Mssages::update_last_update(QString username,QString sender_update,QString RoomName, QString date)
{


    QSqlQuery query_update_last_update(db);

    query_update_last_update.prepare("UPDATE "+
                                     username+" SET lastMessage_Info = :date , updateSender = :up WHERE Rooms = :room");

    query_update_last_update.bindValue(":date",date);
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

    query_update_last_update.finish();
    query_update_last_update.value("lastupdate").toString();
    return UPDATE_LAST_DATE_MESSAGE_SUCCESSFULLY;
}



QStringList Client_Mssages::sendForRoomClients(QMap<QString,QTcpSocket*>& clients,QString lastupdate,TextMessage msg,QString tableName)
{


    QSqlQuery query_SendRommClients(db);



    query_SendRommClients.prepare("SELECT "+
                                  msg.getReciever()+" FROM "+tableName+" WHERE "+msg.getReciever()+" != :sender");

    query_SendRommClients.bindValue(":sender",msg.getSender());

    if(!query_SendRommClients.exec())
    {

        QStringList roomData = msg.getReciever().split("_");
        if(roomData[0] == "pv")
        {

            roomData.swap(1,2);

            QString tempRoom = roomData.join("_");

            query_SendRommClients.clear();

            query_SendRommClients.prepare("SELECT "+
                                          tempRoom+" FROM "+tableName+" WHERE "+tempRoom+" != :sender");

            query_SendRommClients.bindValue(":sender",msg.getSender());

            if(!query_SendRommClients.exec())
            {
                QMessageBox *m= new QMessageBox();
                m->setText("sendForRoomClients  -> swaped : "+query_SendRommClients.lastError().text());
                m->exec();
                delete m;

                query_SendRommClients.clear();
            }

        }
        else
        {
            QMessageBox *m= new QMessageBox();
            m->setText("sendForRoomClients: "+query_SendRommClients.lastError().text());
            m->exec();
            delete m;

            query_SendRommClients.clear();
            //return DATABASE_ERROR;
        }
    }


    QString RoomName = msg.getReciever();

    QStringList log;
    while (query_SendRommClients.next()) {

        QString recieverName = query_SendRommClients.value(msg.getReciever()).toString();

        if(clients.contains(recieverName))
        {
            if(!IsUpdateData(recieverName,msg.getReciever(),lastupdate))
            {

                QByteArray buf = getupdates(get_LastUpdate(recieverName,msg.getReciever()),msg);

                clients[recieverName]->write(buf);
                if( clients[recieverName]->waitForBytesWritten())
                {
                    short err =  update_last_update(recieverName,msg.getSender(),RoomName,msg.gettimeSend().toString("yyyy.MM.dd-hh:mm:ss.zzz"));

                }
                QString temp = msg.getSender() + " -> " + recieverName+" in "+msg.gettimeSend().toString("yyyy.MM.dd / hh:mm:ss")+" : \n( "
                        +msg.getMessage()+" )";
                log.append(temp);

            }

        }
    }

    query_SendRommClients.finish();

    return log;
    // return UPDATE_LAST_DATE_MESSAGE_SUCCESSFULLY;

}

short Client_Mssages::receiveUpdates(QMap<QString,QTcpSocket*> clients,QSqlDatabase Mydb, QString receiver, QString RoomName)
{
    QSqlQuery query_last_updateSender(db);
    query_last_updateSender.prepare("SELECT MAX(Date) FROM "+RoomName);

    if(!query_last_updateSender.exec())
    {
        QMessageBox *m= new QMessageBox();
        m->setText("receiveUpdates : "+query_last_updateSender.lastError().text());
        m->exec();
        delete m;

        query_last_updateSender.finish();
        return Client_Mssages::DATABASE_ERROR;
    }
    QString lastUpdate;
    if(query_last_updateSender.next())
    {
        lastUpdate = query_last_updateSender.value(0).toString();
        query_last_updateSender.finish();
    }
    else{
        query_last_updateSender.finish();
        return Client_Mssages::DATABASE_ERROR;
    }



    if(!IsUpdateData(receiver,RoomName,lastUpdate))
    {
        TextMessage msg;
        msg.setReceiver(RoomName);
        msg.setSender("");
        QByteArray buf = getupdates(lastUpdate,msg);
        clients[receiver]->write(buf);
        if( clients[receiver]->waitForBytesWritten())
            update_last_update(receiver,receiver,RoomName,msg.gettimeSend().toString("yyyy.MM.dd-hh:mm:ss.zzz"));


    }

    // return UPDATE_LAST_DATE_MESSAGE_SUCCESSFULLY;
    return Client_Mssages::MESSAGE_SUCCESSFULLY_SENDED;


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

    query_getLastUpdate.prepare("SELECT * FROM "+username+" WHERE Rooms = :roomname");

    query_getLastUpdate.bindValue(":roomname",RoomName);

    if(!query_getLastUpdate.exec())
    {
        QMessageBox *m= new QMessageBox();
        m->setText("get_lastUpdate: "+query_getLastUpdate.lastError().text());
        m->exec();
        delete m;

        query_getLastUpdate.clear();
        exit(1);

    }


    if(query_getLastUpdate.next())
    {
        return  query_getLastUpdate.value("lastMessage_Info").toString();
    }
    query_getLastUpdate.finish();
    return "";

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
            query_check_last_update.finish();
            return  false;
        }
        else if(query_check_last_update.value("lastMessage_Info").toString() == lastSenderUpdate)
        {
            query_check_last_update.finish();
            return true;
        }
        else
        {
            query_check_last_update.finish();
            return false;
        }
    }
    query_check_last_update.finish();
    return false;
}

QByteArray Client_Mssages::getupdates( QString lastUserUpdate, TextMessage msg)
{


    QSqlQuery query_getUpdates(db);

    if(lastUserUpdate == "")
    {

        query_getUpdates.prepare("SELECT * FROM "+msg.getReciever()/*+" WHERE date >= :d "ORDER BY date DESC"*/);

    }
    else
        query_getUpdates.prepare("SELECT * FROM "+msg.getReciever()+" WHERE date > :d "
                                                                    "ORDER BY date DESC");


    query_getUpdates.bindValue(":d",lastUserUpdate);


    if(!query_getUpdates.exec())
    {
        QMessageBox *m= new QMessageBox();
        m->setText(query_getUpdates.lastError().text());
        m->exec();
        delete m;

        query_getUpdates.clear();
        exit(1);
    }


    QJsonArray messagesArray;


    while(query_getUpdates.next())
    {

        QJsonObject obj;
        obj.insert("username",query_getUpdates.value("name").toString());
        obj.insert("message",query_getUpdates.value("message").toString());
        obj.insert("date",query_getUpdates.value("date").toString());
        obj.insert("isFile",query_getUpdates.value("isFile").toBool());

        messagesArray.append(obj);

        //        QJsonArray data;
        //        QString messageNew = query_getUpdates.value("message").toString();

        //        data.append(messageNew);
        //        QString timeNew = query_getUpdates.value("date").toString();
        //        data.append(timeNew);

        //        objs.insert(query_getUpdates.value("name").toString(),data);

        //        objs.insert("isFile",query_getUpdates.value("isFile").toBool());


    }
    QJsonDocument docMessages;
    docMessages.setArray(messagesArray);
    query_getUpdates.finish();
    return  docMessages.toJson();
}

template<class T>
short Client_Mssages::addDataInColumn(T data, QString ColName, QString tableName)
{

    QSqlQuery query_addDataInColumn(db);
    if(data[0] == "pv")
    {

        unsigned long long int i=1;
        data.removeAt(0);
        foreach(QString x,data){

            query_addDataInColumn.prepare(
                        "UPDATE "+tableName+" SET "+ColName+" = :x WHERE ID = :i;"
                        );
            query_addDataInColumn.bindValue(":x",x);
            query_addDataInColumn.bindValue(":i",i);
            if(!query_addDataInColumn.exec())
            {
                sendmessage("query_addDataInColumn:"+query_addDataInColumn.lastError().text());
                query_addDataInColumn.clear();
                return DATABASE_ERROR;

            }
            i++;

        }
    }
    else
    {

    }

    query_addDataInColumn.finish();
    return ADD_DATA_SUCCESSFULLY;

}


short Client_Mssages::addData_inPersonalTable(QString username,QString RoomName)
{
    QSqlQuery query_addData_inPersonalTable(db);

    query_addData_inPersonalTable.prepare(

                "INSERT INTO "+username+
                " (Rooms,lastMessage_Info,updateSender) "
                "VALUES (:r,NULL,NULL)"
                );

    query_addData_inPersonalTable.bindValue(":r",RoomName);

    if(!query_addData_inPersonalTable.exec())
    {
        QMessageBox *m= new QMessageBox();
        m->setText("addData_inPersonalTable : "+query_addData_inPersonalTable.lastError().text());
        m->exec();
        delete m;

        //query_addData_inPersonalTable.clear();
        query_addData_inPersonalTable.finish();
        return Client_Mssages::DATABASE_ERROR;
    }

    query_addData_inPersonalTable.finish();
    return ADD_DATA_SUCCESSFULLY;

}
