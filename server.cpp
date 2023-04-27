
#include "server.h"
#include "ui_server.h"
#define PORT 9999
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

    QSqlQuery query;
    query.prepare("SELECT * FROM mytb");


    if (!query.exec()) {
        QString Error = "Failed to execute query:" + query.lastError().text();
        QMessageBox::critical(this,"!",Error);
    } else {
        while (query.next()) {
            int field1 = query.value(0).toInt();
            QString field2 = query.value(1).toString();
            QString ans = "Record: " + field2 +" " + QString::number(field1);
            QMessageBox::information(this,"data",ans);
        }
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

    ui->lbl->setText(tr("The server is running on\n\nIP: %1\nport: %2\n\n"
                        "Run the Fortune Client example now.")
                     .arg(ipAddress).arg(tcpServer->serverPort()));


    /*fortunes << tr("You've been leading a dog's life. Stay off the furniture.")
             << tr("You've got to think about tomorrow.")
             << tr("You will be surprised by a loud noise.")
             << tr("You will feel hungry again in another hour.")
             << tr("You might have mail.")
             << tr("You cannot kill time without injuring eternity.")
             << tr("Computers are not intelligent. They only think they are.");*/


    connect(tcpServer, &QTcpServer::newConnection, this, &server::ProgressOfClients);


}

server::~server()
{
    delete ui;
}

void server::ProgressOfClients()
{
    //QMessageBox::information(this,"","connected");
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    //out << fortunes[QRandomGenerator::global()->bounded(fortunes.size())];

    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();


    connect(clientConnection, &QAbstractSocket::disconnected,
            clientConnection, &QObject::deleteLater);

    clientConnection->write(block);
    clientConnection->disconnectFromHost();
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
