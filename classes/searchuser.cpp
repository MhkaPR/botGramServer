#include "searchuser.h"


#define OPTIONS_USER 2
searchUserPackat::searchUserPackat()
{
    header = SEARCHUSER;

}

void searchUserPackat::deserialize(QByteArray buffer)
{
    QDataStream in(&buffer,QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_0);

    qint64 headerMe;

    in >> headerMe >> email>>username >> name;



    header =static_cast<HEADERS>(headerMe);
}

QByteArray searchUserPackat::serialize()
{
    QByteArray buf;
    QDataStream out(&buf,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    out << static_cast<short>(header) << email << username << name;

    return  buf;
}

void searchUserPackat::setemail(QString value){
    email = value;
}
QString searchUserPackat::getemail(){
    return email;
}




short searchUserPackat::findUser(QSqlDatabase db, QString tableName)
{
    QSqlQuery query_findUser(db);

    query_findUser.prepare("SELECT * FROM "+tableName+" WHERE email = :e");
    query_findUser.bindValue(":e",email);


    if(!query_findUser.exec())
    {
        QMessageBox *m= new QMessageBox();
        m->setText("findUser:"+query_findUser.lastError().text());
        m->exec();
        delete m;


        query_findUser.finish();
        return DATABASEERROR;
    }
    if(query_findUser.next())
    {

        username = query_findUser.value(0).toString();
        name = query_findUser.value(3).toString();

        return SUCCESSFULYFINDED;

    }

    return NOTFOUND;

}

