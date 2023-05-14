#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>
#include <QDialog>
#include <QLabel>
#include <QList>
#include <QMap>

#include <QTcpServer>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QRandomGenerator>
#include <QAbstractSocket>
#include <QTcpSocket>

//database
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQueryModel>

#include "classes/database.h"
#include "classes/verify.h"
#include "classes/authentication.h"
#include "classes/tokenbuilder.h"
#include "classes/client_mssages.h"
#include "classes/room.h"
#include "classes/roompacket.h"

QT_BEGIN_NAMESPACE
namespace Ui { class server; }
QT_END_NAMESPACE

class server : public QMainWindow
{
    Q_OBJECT


public:
    server(QWidget *parent = nullptr);
    inline void sendmessage(QString str);
    ~server();

private slots:
    void ProgressOfClients();
    void PacketsHandle();
    void disConnectClient();
private:
    Ui::server *ui;

    void initServer();
    bool connectToDB(const QString name);

    QSqlDatabase mydb;
    QString DB_Name = "ServerDb.db";
    QTcpServer *tcpServer = nullptr;
    QList<QString> fortunes;


    QMap<QString,QTcpSocket*> Clients;
    QList<QTcpSocket*> clients;
    QList<QString> Clients_Username;

};
#endif // SERVER_H
