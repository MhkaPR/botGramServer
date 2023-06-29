#include "server.h"
#include "ui_server.h"
#include "classes/package.h"
#include "classes/textmessage.h"
#include "classes/connectverify.h"
#include "classes/loginpacket.h"
#include "classes/systemmessagepacket.h"
#include "classes/adduser_spacket.h"
#include "classes/tokenpacket.h"
#include "classes/filemessage.h"



#define PORT 9999
#define MAX_LENGTH_DATA 55*1024
server::server(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::server)
{

    ui->setupUi(this);

    //connect to database
    if(!connectToDB(DB_Name))
    {
        QApplication::quit();
    }

    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any,PORT)) {
        QMessageBox::critical(this, tr("Fortune Server"),
                              tr("Unable to start the server: %1.")
                              .arg(tcpServer->errorString()));
        close();
        return;
    }

    QString ipAddress;
    const QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (const QHostAddress &entry : ipAddressesList) {
        //        ipAddress = entry.toString();
        //        ui->plainTextEdit->appendPlainText(tr("The server is running on\n\nIP: %1\nport: %2\n\n")
        //                         .arg(ipAddress).arg(tcpServer->serverPort()));
        if (entry != QHostAddress::LocalHost && entry.toIPv4Address()) {
            ipAddress = entry.toString();

            break;


        }

    }


    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

    ui->lbl->setText(tr("The server is running on\n\nIP: %1\nport: %2\n\n")
                     .arg(ipAddress).arg(tcpServer->serverPort()));





    connect(tcpServer, &QTcpServer::newConnection, this, &server::ProgressOfClients);


}

inline void server::sendmessage(QString str)
{
    QMessageBox::information(this,"message",str);
}

server::~server()
{
    delete ui;

    QString pythonScript = "freeMemory.py";

    QProcess process;
    process.start("python", QStringList() << pythonScript);

    process.waitForFinished();
    qDebug() << "Memory Free!";
}
bool server::connectToDB(const QString name)
{
    mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setHostName("localhost");
    mydb.setDatabaseName(name);

    if (!mydb.open()) {
        QString error =  "Failed to connect to database:" + mydb.lastError().text();
        QMessageBox::critical(this,"dataBase Error",error);
        return false;
    } else {
        return true;
    }
}

void server::PacketsHandle()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if(!clientSocket)
    {
        ui->plainTextEdit->appendPlainText("Error in recieve message of client");
    }
    else
    {



        QByteArray buffer ;
        QDataStream in(&buffer,QIODevice::ReadOnly);
        in.setVersion(QDataStream::Qt_4_0);
        buffer = clientSocket->read(MAX_LENGTH_DATA); // get maximum 55 kb

        qDebug() << "first data >> "<<buffer[0];
        if(buffer[0] == '~') return;
        else if(buffer[0] == '#')
        {

        }
        short header;  in >> header;

        ui->plainTextEdit->appendPlainText(QString::number(header));



        //check header
        switch (header) {
        case package::CONNECT:
        {
            connectVerify Conn;
            Conn.deserialize(buffer);
            if(Client_Mssages::ConnectConfrime(mydb,Conn.Token) == Client_Mssages::USER_FOUND_OK)
            {
                Clients[Conn.Token] = clientSocket;

                ui->plainTextEdit->appendPlainText(Conn.Token+" Connected ----------------");

                clientSocket->write("#");
                clientSocket->waitForBytesWritten();


            }
            else
            {

                clientSocket->disconnectFromHost();

                if(clientSocket->state() == QAbstractSocket::UnconnectedState
                        || clientSocket->waitForDisconnected(5000));

            }
            break;
        }
        case package::UPDATE_CLIENT:
        {

            updateClient updateMessages(mydb);
            updateMessages.deserialize(buffer);
            qDebug() <<updateMessages.IsApply;
            if(updateMessages.IsApply)
            {
                updateMessages.fixUpdates(Clients.key(clientSocket));
                updateMessages.IsApply=0;

                QByteArray bufferOfnewUpdateMessages;
                QDataStream outUpdate(&bufferOfnewUpdateMessages,QIODevice::WriteOnly);
                outUpdate.setVersion(QDataStream::Qt_4_0);
                outUpdate<<static_cast<short>(updateMessages.getheader())<<updateMessages.serialize();
                //sendmessage(updateMessages.getDocJson().toJson());
                clientSocket->write(bufferOfnewUpdateMessages);
                clientSocket->waitForBytesWritten();
                qDebug() << "data sent!";
                ///clientSocket->waitForReadyRead();


            }
            else
            {

            }


            break;
        }
        case package::VERIFY:
        {
            ui->plainTextEdit->appendPlainText("someone wants to verify");
            loginPacket loginData;
            loginData.deserialize(buffer);
            Verify myVerify(mydb,loginData);

            ui->plainTextEdit->appendPlainText(QString::number(myVerify.IsLogin));
            //sendmessage(loginData.getJsonLoginData());
            if(myVerify.IsLogin)
            {
                systemMessagePacket SysMsg;
                SysMsg.setSysmsg(myVerify.Login());
                ui->plainTextEdit->appendPlainText(QString::number(static_cast<short>(SysMsg.getSysmsg())));
                QByteArray answerBuf;
                QDataStream out(&answerBuf,QIODevice::WriteOnly);
                out.setVersion(QDataStream::Qt_4_0);
                QSqlQuery query;
                query.prepare("SELECT email , Name, Token FROM Users_Information WHERE username=:u ");
                query.bindValue(":u",myVerify.username);
                // Execute the query
                if (!query.exec()) {
                    qDebug() << "Failed to execute query!";
                    query.finish();

                    return;
                }
                if(query.next())
                {
                    QJsonDocument jsondoc;
                    QJsonObject jsonobj;
                    jsonobj.insert("email",query.value("email").toString());
                    jsonobj.insert("name",query.value("Name").toString());
                    jsonobj.insert("token",query.value("Token").toString());
                    jsondoc.setObject(jsonobj);
                    SysMsg.setInformation(jsondoc.toJson());
                }
                out << static_cast<short>( SysMsg.getheader())<<SysMsg.serialize();

                clientSocket->write(answerBuf);

            }
            else
            {
                systemMessagePacket SysMsg;
                SysMsg.setSysmsg(myVerify.checkForSignIn());

                QByteArray answerBuf;
                QDataStream out(&answerBuf,QIODevice::WriteOnly);
                out.setVersion(QDataStream::Qt_4_0);

                out << static_cast<short>(SysMsg.getheader())<<SysMsg.serialize();
                // sendmessage(QString::number(static_cast<short>(SysMsg.getSysmsg())));
                clientSocket->write(answerBuf);

                QSqlQuery query_createTableForNewClient(mydb);
                query_createTableForNewClient.prepare("CREATE TABLE "+myVerify.username+" (ID INTEGER,Rooms TEXT,lastMessage_Info TEXT,updateSender TEXT)");

                if(!query_createTableForNewClient.exec())
                {
                    QMessageBox::information(this,"warning","Execute the query","ok");
                    return;
                }
                query_createTableForNewClient.finish();


            }
            break;
        }
        case package::SYSTEM:
        {


            systemMessagePacket sysMsg;

            sysMsg.deserialize(buffer);

            switch (sysMsg.getSysmsg()) {
            case package::Send_VerifyCode:
            {


                QByteArray answerBuf;
                QDataStream out(&answerBuf,QIODevice::WriteOnly);
                out.setVersion(QDataStream::Qt_4_0);

                Authentication myAutho;


                CheckVerifySafePacket safepackate = myAutho.getSafeVerify();

                out << static_cast<short>(safepackate.getheader())<<safepackate.serialize();
                // sendmessage(safepackate.getAnswer());
                clientSocket->write(answerBuf);


                break;
            }
            case package::send_file:
            {


                QJsonDocument doc;
                doc = QJsonDocument::fromJson(sysMsg.getInformation());

                QJsonObject obj = doc.object();



                QDir cur = QDir::current();
                //cur.cdUp();
                cur.cd("files/"+obj["room"].toString());

                QString filename = obj["FileName"].toString();
                QString filename_AND_Address = cur.path()+"/"+obj["room"].toString()+"---"+filename;
                QFile *filefound = new QFile(filename_AND_Address);

                if(filefound->exists())
                {
                    if(filefound->open(QIODevice::ReadOnly))
                    {

                        fileMessage fmsg(Clients.key(clientSocket));
                        fmsg.setroom(obj["room"].toString());
                        fmsg.setFileName(filename);
                        fmsg.settimeSend(QDateTime::fromString(obj["FileName"].toString().left(17)));
                        fmsg.setcount_size("0");
                        fmsg.sendFile(filefound,clientSocket);


                    }


                }
                else
                {
                    sendmessage("file not found");
                    delete filefound;
                }




                break;
            }
            case package::get_name:
            {

                QJsonDocument jsondoc = QJsonDocument::fromJson(sysMsg.getInformation());
                QJsonObject jsonobj;
                jsonobj =jsondoc.object();

                QSqlQuery query(mydb);
                query.prepare("UPDATE Users_Information SET  Name=:n WHERE username=:u");

                qDebug() << jsonobj["name"].toString();
                query.bindValue(":n",jsonobj["name"].toString());
                query.bindValue(":u",jsonobj["username"].toString());

                // Execute the query
                if (!query.exec()) {
                    QMessageBox::information(this,"warning",query.lastError().text(),"ok");

                    return;
                }

                query.finish();
                mydb.commit();
                clientSocket->write("~");
                clientSocket->waitForBytesWritten();
                break;
            }
            case package::update_In_last_in_Rooms:
            {
                qDebug() << "update_In_last_in_Rooms...";
                QString username = Clients.key(clientSocket);
                qDebug() << username;


                // get name of Rooms that username have
                QSqlQuery queryGetRooms(mydb);
                queryGetRooms.prepare("SELECT Rooms FROM "+username);
                if(!queryGetRooms.exec())
                {
                    qDebug() << "Failed to execute query! getRooms -->"+queryGetRooms.lastError().text();
                    queryGetRooms.finish();

                    return;
                }
                while (queryGetRooms.next())
                {


                    QString RoomName = queryGetRooms.value("Rooms").toString();
                    qDebug() << " We are in Room :"<< RoomName;

                    //from each Room get last message date and sender name
                    QSqlQuery queryGetLastUpdatesANDFixUpdates(mydb);

                    queryGetLastUpdatesANDFixUpdates.prepare( "SELECT max(date) , name FROM "+RoomName);
                    bool ans = queryGetLastUpdatesANDFixUpdates.exec();
                    if(!ans)
                    {
                        qDebug() << "Failed to execute query! getlastUpdates -->"+queryGetLastUpdatesANDFixUpdates.lastError().text();
                        queryGetLastUpdatesANDFixUpdates.finish();
                        return;
                    }


                    if(queryGetLastUpdatesANDFixUpdates.next())
                    {
                        QString date = queryGetLastUpdatesANDFixUpdates.value("max(date)").toString();
                        QString senderName = queryGetLastUpdatesANDFixUpdates.value("name").toString();

                        //next date and sender name update in user table

                        qDebug() << date<< senderName;
                        queryGetLastUpdatesANDFixUpdates.clear();
                        queryGetLastUpdatesANDFixUpdates.finish();


                        queryGetLastUpdatesANDFixUpdates.prepare("UPDATE "+username+
                                                                 " SET lastMessage_Info = :d "
                                                                 " , updateSender = :u WHERE Rooms = :R");
                        queryGetLastUpdatesANDFixUpdates.bindValue(":d",date);
                        queryGetLastUpdatesANDFixUpdates.bindValue(":u",senderName);
                        queryGetLastUpdatesANDFixUpdates.bindValue(":R",RoomName);

                        ans = queryGetLastUpdatesANDFixUpdates.exec();

                        if(!ans)
                        {
                            qDebug() << "Failed to execute query! update in user table -->"+queryGetLastUpdatesANDFixUpdates.lastError().text();
                            queryGetLastUpdatesANDFixUpdates.finish();

                            return;
                        }
                        queryGetLastUpdatesANDFixUpdates.finish();


                    }




                }// end loop of get names of rooms
                clientSocket->write("~");
                clientSocket->waitForBytesWritten();
                break;
            }
            default:break;
            }
            break;
        }
        case package::ADDUSER_TO_USERS_DATABASE:
        {
            AddUser_SPacket user;

            user.deserialize(buffer);


            //decode data...


            QByteArray answerBuf;
            QDataStream out(&answerBuf,QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_4_0);


            TokenBuilder T(DB_Name,mydb);
            TokenPacket Clienttoken;
            Clienttoken.setToken(T.token());
            out << static_cast<short>(Clienttoken.getheader())<<Clienttoken.serialize();

            loginPacket userL;
            userL.setJsonLoginData(user.getdata());

            Verify myVerify(mydb,userL);
            if(!myVerify.addNewUser(T.token()))
            {
                exit(1);
            }
            userInfo SignedUser;
            SignedUser=myVerify.UserInformation();
            ui->plainTextEdit->appendPlainText("added user :\n" +
                                               SignedUser.username
                                               +" // "
                                               +SignedUser.password
                                               +" // "
                                               +SignedUser.email
                                               +" // Token: "
                                               +T.token());


            clientSocket->write(answerBuf);


            break;
        }
        case package::TEXTMESSAGE:
        {

            TextMessage msg;

            msg.deserialize(buffer);


            //sendmessage(msg.Message);
            Client_Mssages messageProc(msg);

            if(messageProc.MessageConfrime(mydb) == Client_Mssages::USER_FOUND_OK)
            {

                QString RoomName =msg.getReciever();

                QString senderUsername=Client_Mssages::getUsername(msg.getSender(),mydb);
                msg.setSender(senderUsername);
                //sasions for send correctly messages

                //add message in pv_1

                messageProc.add_in_Room(&RoomName,senderUsername,
                                        msg.gettimeSend().toString("yyyy.MM.dd-hh:mm:ss.zzz"),msg.getMessage(),false);

                msg.setReceiver(RoomName);
                //update last update sende

                messageProc.update_last_update(senderUsername,senderUsername,RoomName
                                               ,msg.gettimeSend().toString("yyyy.MM.dd-hh:mm:ss.zzz"));


                // While for other clients

                QString lastUpdate = messageProc.get_LastUpdate(Clients.key(clientSocket),msg.getReciever());

                QStringList logs = messageProc.sendForRoomClients(Clients,lastUpdate,msg);
                foreach (QString log, logs) {
                    ui->plainTextEdit->appendPlainText(log);
                }


            }
            break;
        }
        case package::FILEMESSAGE:
        {

            static quint64 count = 1;
            //sendmessage(QString::number(buffer.length()));
            //sendmessage(QString::number(buffer.length()));
            fileMessage fmsg(Clients.key(clientSocket));

            fmsg.deserialize(buffer);

            static quint64 bufsize=0;
            bufsize += static_cast<quint64>(buffer.size());

            //            static QByteArray bufRam;
            //            qint32 bufsize = bufRam.size();

            //            bufRam.append(fmsg.getData());
            //
            //datasInRam[fmsg.getroom()][fmsg.getFileName()].append(fmsg.getData());// add part of file in ram







            QDir cur(QDir::current());
            // cur.cdUp();
            cur.cd("files/"+fmsg.getroom());




            QFile fileReceiive(cur.path()+"/"+fmsg.getroom()+"---"+fmsg.getFileName());


            if(!fileReceiive.open(QIODevice::Append | QIODevice::WriteOnly))
            {
                sendmessage(fileReceiive.errorString());
                exit(1);
            }
            fileReceiive.write(fmsg.getData());
            ui->plainTextEdit->appendPlainText("loaded "+QString::number(bufsize));
            //fileReceiive.flush()
            fileReceiive.close();





            if(fmsg.IsEndFile())
            {



                Client_Mssages messageProc(static_cast<TextMessage>(fmsg));

                //                    short errr= static_cast<short>(messageProc.MessageConfrime(mydb));
                //                    sendmessage(QString::number(errr)+"\n"+fmsg.getSender());
                //                if(errr == static_cast<short>( Client_Mssages::USER_FOUND_OK))

                //                {

                QString RoomName =fmsg.getroom();

                //                    QString senderUsername=Client_Mssages::getUsername(fmsg.getSender(),mydb);
                //                    fmsg.setSender(senderUsername);
                //sasions for send correctly messages

                //add message in pv_1
                // qDebug() << RoomName;
                //qDebug()<<senderUsername;
                messageProc.add_in_Room(&RoomName,fmsg.getSender(),
                                        fmsg.gettimeSend().toString("yyyy.MM.dd-hh:mm:ss.zzz"),fmsg.getFileName(),true);

                fmsg.setroom(RoomName);
                //update last update sender

                messageProc.update_last_update(fmsg.getSender(),fmsg.getSender(),RoomName
                                               ,fmsg.gettimeSend().toString("yyyy.MM.dd-hh:mm:ss.zzz"));


                // While for other clients



                QString lastUpdate = messageProc.get_LastUpdate(Clients.key(clientSocket),fmsg.getroom());



                QStringList logs = messageProc.sendForRoomClients(Clients,lastUpdate,static_cast<TextMessage>(fmsg));
                foreach (QString log, logs) {
                    ui->plainTextEdit->appendPlainText(log);
                    //}


                }



                bufsize = 0;

            }
            else {


            }

            QString temp = "~";
            clientSocket->write(temp.toStdString().c_str());
            clientSocket->waitForBytesWritten();


            count++;
            break;

        }
        case package::SEARCHUSER:
        {
            searchUserPackat search;
            search.deserialize(buffer);

            short err = search.findUser(mydb);


            QByteArray buf_searchAns;
            QDataStream out_searchAns(&buf_searchAns,QIODevice::WriteOnly);
            out_searchAns.setVersion(QDataStream::Qt_4_0);

            if(err != searchUserPackat::DATABASEERROR)
            {

                //send Answer
                out_searchAns << static_cast<short>(search.getheader()) << search.serialize();
                clientSocket->write(buf_searchAns);
                clientSocket->waitForBytesWritten();

            }
            else exit(1);

            break;

        }


        default:break;
        }
    }
}


void server::ProgressOfClients()
{
    //    //QMessageBox::information(this,"","connected");
    //    QByteArray block;
    //    QDataStream out(&block, QIODevice::WriteOnly);
    //    out.setVersion(QDataStream::Qt_4_0);

    //    //out << fortunes[QRandomGenerator::global()->bounded(fortunes.size())];

    //    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();


    //    connect(clientConnection, &QAbstractSocket::disconnected,
    //            clientConnection, &QObject::deleteLater);

    //    clientConnection->write(block);
    //    clientConnection->disconnectFromHost();

    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();

    ui->plainTextEdit->appendPlainText(QString::number(clientConnection->socketDescriptor()));



    //clients.append(clientConnection);
    //Clients.insert(clientConnection,"");

    connect(clientConnection,&QAbstractSocket::readyRead,this,&server::PacketsHandle);
    connect(clientConnection, &QAbstractSocket::disconnected,clientConnection, &QObject::deleteLater);
    connect(clientConnection, &QAbstractSocket::disconnected,this,&server::disConnectClient);
    //clientConnection->disconnectFromHost();


}
void server::disConnectClient()
{

    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    ui->plainTextEdit->appendPlainText(Clients.key(clientSocket)+"("+QString::number(clientSocket->socketDescriptor())+") Disconnected------------------------------");
    Clients.remove(Clients.key(clientSocket));
}

