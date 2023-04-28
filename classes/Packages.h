#ifndef PACKAGES_H
#define PACKAGES_H
#include <QObject>
#include <QJsonDocument>
enum HEADERS : short
{
    VERIFY,
    SYSTEM,

};
enum SysCodes : short
{
    login_confrimed,
    username_not_found,
    password_is_not_correct,

};

struct headerPackage
{

    HEADERS header;

};

struct loginPacket : headerPackage
{
    QByteArray JsonInformation;
    loginPacket() {
        header = VERIFY;
    }

    friend QDataStream &operator<<(QDataStream &out,const loginPacket &data)
       {
            out << static_cast<short>(data.header) << data.JsonInformation;
           return out;
       }

       friend QDataStream &operator>>(QDataStream &in, loginPacket &data)
       {
           short headerAsint;
           in >> headerAsint >> data.JsonInformation;
           data.header = static_cast<HEADERS>(headerAsint);
           return in;
       }
};
struct systemMessagePacket : headerPackage
{

    SysCodes msg;

    systemMessagePacket() {
        header = SYSTEM;
    }

    friend QDataStream &operator<<(QDataStream &out,const systemMessagePacket &data)
       {
            out << static_cast<short>(data.header) << static_cast<short>(data.msg);
           return out;
       }

       friend QDataStream &operator>>(QDataStream &in, systemMessagePacket &data)
       {
           short headerAsint;
           short msgAsshort;
           in >> headerAsint >> msgAsshort;
           data.header = static_cast<HEADERS>(headerAsint);
           data.msg = static_cast<SysCodes>(msgAsshort);
           return in;
       }
};
#endif // PACKAGES_H
