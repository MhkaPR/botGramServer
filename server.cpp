

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


        short header;  in >> header;


        // package *p=nullptr;


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


                //get query of Rooms with lat update


                updateMessagePacket updates;


                updates.setDb(mydb);
                updates.receiveRoomsData(Conn.Token);


                //send one room data
                //get answer of client to send nxt room data


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
            //

            break;
        }
        case package::VERIFY:
        {

            loginPacket loginData;
            loginData.deserialize(buffer);
            Verify myVerify(mydb,loginData);

            if(myVerify.IsLogin)
            {
                systemMessagePacket SysMsg;
                SysMsg.setSysmsg(myVerify.Login());

                QByteArray answerBuf;
                QDataStream out(&answerBuf,QIODevice::WriteOnly);
                out.setVersion(QDataStream::Qt_4_0);

                out << package::Packeting( SysMsg.getheader(),SysMsg.serialize());

                clientSocket->write(answerBuf);

            }
            else
            {
                systemMessagePacket SysMsg;
                SysMsg.setSysmsg(myVerify.checkForSignIn());

                QByteArray answerBuf;
                QDataStream out(&answerBuf,QIODevice::WriteOnly);
                out.setVersion(QDataStream::Qt_4_0);

                out << package::Packeting(SysMsg.getheader(),SysMsg.serialize());

                clientSocket->write(answerBuf);

            }
            break;
        }
        case package::SYSTEM:
        {
            systemMessagePacket sysMsg;
            short msg;
            in >> msg;
            sysMsg.setSysmsg(static_cast<package::SysCodes>(msg));


            switch (sysMsg.getSysmsg()) {
            case package::Send_VerifyCode:
            {


                QByteArray answerBuf;
                QDataStream out(&answerBuf,QIODevice::WriteOnly);
                out.setVersion(QDataStream::Qt_4_0);

                Authentication myAutho;


                CheckVerifySafePacket safepackate = myAutho.getSafeVerify();

                out << package::Packeting(safepackate.getheader(),safepackate.serialize());

                clientSocket->write(answerBuf);


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
            out << package::Packeting(Clienttoken.getheader(),Clienttoken.serialize());

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
                //update last update sender

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
            cur.cdUp();
            cur.cd("serverTest01");
            cur.cd("files");




            QFile fileReceiive(cur.path());

            fileReceiive.setFileName(fmsg.getroom()+"---"+fmsg.getFileName());
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


                if(messageProc.MessageConfrime(mydb) == Client_Mssages::USER_FOUND_OK)
                {

                    QString RoomName =fmsg.getroom();

                    QString senderUsername=Client_Mssages::getUsername(fmsg.getSender(),mydb);
                    fmsg.setSender(senderUsername);
                    //sasions for send correctly messages

                    //add message in pv_1

                    messageProc.add_in_Room(&RoomName,senderUsername,
                                            fmsg.gettimeSend().toString("yyyy.MM.dd-hh:mm:ss.zzz"),fmsg.getFileName(),true);

                    fmsg.setroom(RoomName);
                    //update last update sender

                    messageProc.update_last_update(senderUsername,senderUsername,RoomName
                                                   ,fmsg.gettimeSend().toString("yyyy.MM.dd-hh:mm:ss.zzz"));


                    // While for other clients



                    QString lastUpdate = messageProc.get_LastUpdate(Clients.key(clientSocket),fmsg.getroom());



                    QStringList logs = messageProc.sendForRoomClients(Clients,lastUpdate,static_cast<TextMessage>(fmsg));
                    foreach (QString log, logs) {
                        ui->plainTextEdit->appendPlainText(log);
                    }


                }



                bufsize = 0;

            }
            else {


            }

            QString temp = "~";
            clientSocket->write(temp.toStdString().c_str());
            //clientSocket->flush();


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

