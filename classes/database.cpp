#include "database.h"

DataBase::DataBase()
{

}

void DataBase::sendmessage(QString str)
{
    QMessageBox *msg = new QMessageBox;
    msg->setText(str);
    msg->exec();
}

