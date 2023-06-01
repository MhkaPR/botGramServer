#include "updatemessagepacket.h"




updateMessagePacket::updateMessagePacket(TextMessage message):msg(message) /*Client_Mssages(MessageStruct)*/{

}

void updateMessagePacket::getRoomsData()
{
    QSqlQuery query_getRoomsData(db);

    query_getRoomsData.prepare("SELECT * FROM "+msg.getSender());
    if(!query_getRoomsData.exec())
    {
        QMessageBox *m= new QMessageBox();
        m->setText("get Room Data: "+query_getRoomsData.lastError().text());
        m->exec();
        delete m;


        query_getRoomsData.finish();
        exit(1);
    }

    QByteArray buffer_Update;
    while (query_getRoomsData.next()) {


        QString Roomname = query_getRoomsData.value("Rooms").toString();
        QString lastUpdate = query_getRoomsData.value("lastMessage_Info").toString();

        TextMessage msgTemp;
        Client_Mssages f(msgTemp);
        buffer_Update.append('\n');
        buffer_Update+=f.getupdates(lastUpdate,msg);



    }
}

