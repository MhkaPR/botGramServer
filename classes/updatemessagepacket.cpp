#include "updatemessagepacket.h"




updateMessagePacket::updateMessagePacket()/*Client_Mssages(MessageStruct)*/{

}

void updateMessagePacket::receiveRoomsData(QString sender)
{
    QSqlQuery query_getRoomsData(db);

    query_getRoomsData.prepare("SELECT * FROM "+sender);
    if(!query_getRoomsData.exec())
    {
        QMessageBox *m= new QMessageBox();
        m->setText("get Room Data: "+query_getRoomsData.lastError().text());
        m->exec();
        delete m;


        query_getRoomsData.finish();
        exit(1);
    }

    QByteArray buffer;
    while (query_getRoomsData.next()) {


        QString Roomname = query_getRoomsData.value("Rooms").toString();
        QString lastUpdate = query_getRoomsData.value("lastMessage_Info").toString();

        TextMessage msgTemp;
        msgTemp.setReceiver(Roomname);
        Client_Mssages f(msgTemp);
        buffer.append('\n');
        buffer+=f.getupdates(lastUpdate,msgTemp);

    }
}

void updateMessagePacket::deserialize(QByteArray buffer)
{
    QDataStream in(&buffer,QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_0);

    qint64 headerMe;

    in >> headerMe >> buffer;


    header =static_cast<HEADERS>(headerMe);
}

QByteArray updateMessagePacket::serialize()
{
    QByteArray buf;
    QDataStream out(&buf,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    out << static_cast<short>(header)<< buffer;
    return  buf;
}

