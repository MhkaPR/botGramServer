#include "verify.h"


Verify::Verify(QSqlDatabase db,loginPacket packet) : database(db)
{
    QJsonDocument doc = QJsonDocument::fromJson(packet.JsonInformation);

    QJsonObject objs= doc.object();
    username = objs.value("username").toString();
    password = objs.value("password").toString();
    IsLogin = objs.value("islogin").toBool();
    name = "Users_Information";
}

SysCodes Verify::Login()
{
    QSqlQuery LoginQuery(database);
    LoginQuery.prepare("SELECT * FROM "+
                       name+
                       " WHERE username = :username");
    LoginQuery.bindValue(":username",username);

    //sendmessage(name + " "+ username);

    if(!LoginQuery.exec())
    {
        QString Error = "Failed to execute query:" + LoginQuery.lastError().text();
        //handle error
    }

    if(LoginQuery.next())
    {
        sendmessage(LoginQuery.value(0).toString());

        if(LoginQuery.value("password").toString() == password)
        {

            LoginQuery.clear();
            LoginQuery.prepare("UPDATE "+name+" SET Logined = 1 WHERE username = :username AND password = :password");
            LoginQuery.bindValue(":username",username);
            LoginQuery.bindValue(":password",password);
            if(!LoginQuery.exec())
            {
            sendmessage(LoginQuery.lastError().text());
            }

            return login_confrimed;
        }
        else return password_is_not_correct;
    }
    else return username_not_found;

}
