#include "client_mssages.h"

Client_Mssages::Client_Mssages(TextMessage message) : MessageStruct(message)
{

}

short Client_Mssages::MessageConfrime(QSqlDatabase Db, QString tableName)
{
    db = Db;//to private
    TableName =tableName;//to private
    QSqlQuery queryCheckMessage(db);

    queryCheckMessage.prepare("SELECT Token FROM "+tableName+" WHERE Token = :t");
    queryCheckMessage.bindValue(":t",MessageStruct.getSender());
    if(!queryCheckMessage.exec())
    {
        sendmessage(queryCheckMessage.lastError().text());
        queryCheckMessage.clear();
        return DATABASE_ERROR;
    }
    if(!queryCheckMessage.next()) return USER_NOT_FOUND_WITH_THIS_TOKEN;
    return USER_FOUND_OK;
}

short Client_Mssages::ConnectConfrime(QSqlDatabase Db, QString& Token_username, QString tableName)
{


    QSqlQuery queryCheckMessage(Db);

    queryCheckMessage.prepare("SELECT * FROM "+tableName+" WHERE Token = :t");
    queryCheckMessage.bindValue(":t",Token_username);
    if(!queryCheckMessage.exec())
    {
        QMessageBox *m= new QMessageBox();
        m->setText(queryCheckMessage.lastError().text());
        m->exec();
        delete m;

        queryCheckMessage.clear();
        return DATABASE_ERROR;
    }
    if(!queryCheckMessage.next()) return USER_NOT_FOUND_WITH_THIS_TOKEN;

    Token_username = queryCheckMessage.value("username").toString();
    return USER_FOUND_OK;
}
