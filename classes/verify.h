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


private:
    QSqlDatabase database;
    QString username;
    QString password;
    QString email;
     QString BuildToken();
};


#endif // VERIFY_H
