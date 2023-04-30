
#include "server.h"
#include "ui_server.h"
#include "classes/Packages.h"
#define PORT 9999
#define MAX_LENGTH_DATA 1024
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
    QByteArray buffer ;
    QDataStream in(&buffer,QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_0);
    buffer = clients.last()->read(MAX_LENGTH_DATA); // get maximum 1 kb

    short header;  in >> header;

    //check header
    switch (header) {
    case VERIFY:
    {

        loginPacket loginData;

        in >>loginData.JsonInformation;
        sendmessage(loginData.JsonInformation);
        Verify myVerify(mydb,loginData);

        if(myVerify.IsLogin)
        {
            systemMessagePacket SysMsg;
            SysMsg.msg=myVerify.Login();
            QByteArray answerBuf;
            QDataStream out(&answerBuf,QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_4_0);

            out << SysMsg;

            clients.last()->write(answerBuf);

        }
        else
        {
            systemMessagePacket SysMsg;
            SysMsg.msg = myVerify.checkForSignIn();

            QByteArray answerBuf;
            QDataStream out(&answerBuf,QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_4_0);

            out << SysMsg;

            clients.last()->write(answerBuf);

        }
        break;
    }
    case SYSTEM:
    {
        systemMessagePacket sysMsg;
        short msg;
        in >> msg;
        sysMsg.msg = (SysCodes)msg;
        switch (sysMsg.msg) {
        case Send_VerifyCode:
        {


            QByteArray answerBuf;
            QDataStream out(&answerBuf,QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_4_0);

            Authentication myAutho;
            out << myAutho.getSafeVerify();

            clients.last()->write(answerBuf);


            break;
        }

        default:break;
        }
        break;
    }
    case ADDUSER_TO_USERS_DATABASE:
    {
        AddUser_SPacket user;
        in >>user.data;
        //decode data...


        QByteArray answerBuf;
        QDataStream out(&answerBuf,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_0);
        TokenBuilder T(DB_Name,mydb);
        TokenPacket Clienttoken;
        Clienttoken.Token = T.token();
        out << Clienttoken;

        loginPacket userL;
        userL.JsonInformation = user.data;

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


        clients.last()->write(answerBuf);


        break;
    }
    default:break;
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

    ui->plainTextEdit->appendPlainText("new Connection : "+QString::number(clientConnection->socketDescriptor()));

    clients.append(clientConnection);

    connect(clientConnection,&QAbstractSocket::readyRead,this,&server::PacketsHandle);
    connect(clientConnection, &QAbstractSocket::disconnected,clientConnection, &QObject::deleteLater);
    //clientConnection->disconnectFromHost();


}


