#ifndef VERIFY_H
#define VERIFY_H

#include <QObject>
#include "database.h"
#include "Packages.h"
#include <QMessageBox>



class Verify : public DataBase
{
public:
    Verify(QSqlDatabase db,loginPacket packet);
    bool IsLogin;
    SysCodes Login();
    SysCodes checkForSignIn();
private:
    QSqlDatabase database;
    QString username;
    QString password;
    QString email;
};


#endif // VERIFY_H
