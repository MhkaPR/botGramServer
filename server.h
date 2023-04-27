#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>
#include <QDialog>
#include <QLabel>
#include <QList>

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

QT_BEGIN_NAMESPACE
namespace Ui { class server; }
QT_END_NAMESPACE

class server : public QMainWindow
{
    Q_OBJECT


public:
    server(QWidget *parent = nullptr);
    ~server();

private slots:
    void ProgressOfClients();
private:
    Ui::server *ui;

    void initServer();
    bool connectToDB(const QString name);

    QSqlDatabase mydb;
    QString DB_Name = "ServerDb.db";
    QTcpServer *tcpServer = nullptr;
    QList<QString> fortunes;
};
#endif // SERVER_H
