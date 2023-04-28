#include "verify.h"


Verify::Verify(QSqlDatabase db,loginPacket packet) : database(db)
{
    QJsonDocument doc = QJsonDocument::fromJson(packet.JsonInformation);

    QJsonObject objs= doc.object();
    username = objs.value("username").toString();
    password = objs.value("password").toString();
    email = objs.value("email").toString();
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
        sendmessage("Failed to execute query:" + LoginQuery.lastError().text());
        exit(1);
    }

    if(LoginQuery.next())
    {
        ///sendmessage(LoginQuery.value(0).toString());

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

SysCodes Verify::checkForSignIn()
{
    QSqlQuery LoginQuery(database);
    LoginQuery.prepare("SELECT * FROM "+
                       name+
                       " WHERE username = :username");
    LoginQuery.bindValue(":username",username);


    if(!LoginQuery.exec())
    {
        sendmessage("Failed to execute query:" + LoginQuery.lastError().text());
        exit(1);
        //handle error
    }

    if(LoginQuery.next())
    {
        return s_username_is_repititive;
    }
    else
    {
        LoginQuery.clear();
        LoginQuery.prepare("SELECT * FROM "+
                           name+
                           " WHERE email = :email");
        LoginQuery.bindValue(":email",email);
        if(!LoginQuery.exec())
        {
            sendmessage("Failed to execute query:" + LoginQuery.lastError().text());
            exit(1);
        }
        if(LoginQuery.next()) return s_email_is_repititive;
        else return  s_send_apply_For_Link;
    }
}

bool Verify::addNewUser()
{
    QSqlQuery addUser(database);
    addUser.prepare("INSERT INTO "+name+
                    " (username,password,email,Name,Logined)"
                    " VALUES (:u,:p,:e,NULL,:l)");
    addUser.bindValue(":u",username);
    addUser.bindValue(":p",password);
    addUser.bindValue(":e",email);
    addUser.bindValue(":l",1);
    if(!addUser.exec())
    {
        sendmessage("Failed to execute query:" + addUser.lastError().text());
        return false;
    }
    return true;

}

userInfo Verify::UserInformation()
{
    userInfo user;
    user.username = username;
    user.password = password;
    user.email = email;
    return  user;
}
