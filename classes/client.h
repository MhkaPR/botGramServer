#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QPixmap>
#include <QTcpSocket>

class client
{
public:
    client();

private:
    QString username;
    QString password;
    QString email;
    QString Token;
    QString name;
    QPixmap image;
    qint16 desceriptor;
    QTcpSocket* socket = nullptr;

};

#endif // CLIENT_H
