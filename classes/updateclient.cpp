#include "updateclient.h"

void updateClient::deserialize(QByteArray buffer)
{
    QDataStream in(&buffer,QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_0);

    qint64 headerMe;

    QByteArray jsonFileArray;

    in >> headerMe >> jsonFileArray;

    header =static_cast<HEADERS>(headerMe);

    docJson = QJsonDocument::fromJson(jsonFileArray);

}

QByteArray updateClient::serialize()
{
    QByteArray buf;
    QDataStream out(&buf,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    out << static_cast<short>(header) << docJson.toJson();
    return  buf;
}

updateClient::updateClient()
{

}

QJsonDocument updateClient::getDocJson()
{
    return docJson;

}

void updateClient::setDocJson(QJsonDocument doc)
{
    docJson = doc;
}
