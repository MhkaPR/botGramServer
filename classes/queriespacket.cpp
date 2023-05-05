#include "queriespacket.h"

QueriesPacket::QueriesPacket()
{

}

QString QueriesPacket::getQuery(QString QueryName)
{

    QString nameFile = "../Queries/"+QueryName+".sql";
    QFile file(nameFile);

    if(!file.open(QIODevice::ReadOnly))
    {
        file.close();
        return "ERROR";
    }

    QString data = file.readAll();
    return data;

}
