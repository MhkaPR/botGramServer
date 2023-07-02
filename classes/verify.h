#ifndef VERIFY_H
#define VERIFY_H

#include <QObject>
#include "database.h"
#include "package.h"
#include "loginpacket.h"
#include <QMessageBox>
#include "queriespacket.h"



class Verify : public DataBase
{
public:
    Verify(QSqlDatabase db,loginPacket packet);
    bool IsLogin;
    package::SysCodes Login();
    package::SysCodes checkForSignIn();
    bool addNewUser(QString Token);
    userInfo UserInformation();
    QString username;
    QString password;
    QString email;

private:
    QSqlDatabase database;

     QString BuildToken();
};


#endif // VERIFY_H
