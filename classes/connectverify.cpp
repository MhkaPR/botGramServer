#include "connectverify.h"

connectVerify::connectVerify()
{
    header = package::CONNECT;
}


void connectVerify::deserialize(QByteArray buffer)
{
    QDataStream in(&buffer,QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_0);

    qint64 headerMe;

    QString lastUpdatesTemp;

    in >> headerMe >> Token >> lastUpdatesTemp;

    header =static_cast<HEADERS>(headerMe);

    doc = QJsonDocument::fromJson(lastUpdatesTemp.toStdString().c_str());
}

QByteArray connectVerify::serialize()
{
    QByteArray buf;
    QDataStream out(&buf,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    out << static_cast<short>(header) << Token;
    return  buf;

}

QString connectVerify::getToken()
{
    return Token;

}
