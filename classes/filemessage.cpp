#include "filemessage.h"

fileMessage::fileMessage(QString sender):Sender(sender)
{

    header = FILEMESSAGE;
}

void fileMessage::deserialize(QByteArray buffer)
{
    QDataStream in(&buffer,QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_0);

    qint64 headerMe;

    in >> headerMe >> room >> FileName  >> count_size >> Data;

    timeSend = QDateTime::fromString(FileName.split(".")[0],"yyyyMMddhhmmsszzz");

    header =static_cast<HEADERS>(headerMe);

}

QByteArray fileMessage::serialize()
{
    QByteArray buf;
    QDataStream out(&buf,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    out << static_cast<short>(header)<< room << FileName << count_size << Data;
    return  buf;

}

bool fileMessage::IsEndFile()
{

    if(count_size == "!")
    {
        return true;
    }
    return false;

}

void fileMessage::setroom(QString value){
room = value;
}
QString fileMessage::getroom(){
 return room;
}


void fileMessage::setFileName(QString value){
FileName = value;
}
QString fileMessage::getFileName(){
 return FileName;
}


void fileMessage::setcount_size(QString value){
count_size = value;
}
QString fileMessage::getcount_size(){
 return count_size;
}


void fileMessage::setData(QByteArray value){
Data = value;
}
QByteArray fileMessage::getData(){
    return Data;
}

void fileMessage::setSender(QString value)
{
    Sender = value;
}

QString fileMessage::getSender()
{
    return Sender;
}

void fileMessage::settimeSend(QDateTime value)
{
    timeSend =value;
}

QDateTime fileMessage::gettimeSend()
{
    return timeSend;
}



fileMessage::operator TextMessage()
{
    TextMessage msg;

    msg.setSender(Sender);
    msg.setReceiver(room);
    msg.setbuf(Data);
    msg.setmessage(FileName);
    msg.setIsFile(true);
    msg.timeSend = timeSend;


    return msg;


}

void fileMessage::sendFile(QFile *file, QTcpSocket *socket)
{
    quint64 fileSize = static_cast<quint64>(file->size());
    while (!file->atEnd() /*&& (fmsg.getcount_size() != "!")*/) {

        if(fileSize < 50* 1024 )
        {

            Data = file->read(static_cast<qint64>(fileSize));
            count_size = "!";
            fileSize -= fileSize;
        }
        else
        {

            Data = file->read(50*1024);
            fileSize -= 50*1024;

        }


        QByteArray buf5;
        QDataStream out5(&buf5,QIODevice::WriteOnly);
        out5.setVersion(QDataStream::Qt_4_0);



        out5<<static_cast<short>(header)<<serialize();



       //send pack
        socket->write(buf5);
        socket->waitForBytesWritten();
        qDebug() << "writed " << buf5.size();
        socket->flush();
        socket->waitForReadyRead();

    }
    file->close();
}


