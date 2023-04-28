#ifndef PACKAGES_H
#define PACKAGES_H
#include <QObject>
#include <QJsonDocument>
enum HEADERS : short
{
    VERIFY,
    SYSTEM,
    SAFE_VERIFY,
    addUser_to_UsersDatabase,

};
enum SysCodes : short
{
    login_confrimed,
    username_not_found,
    password_is_not_correct,
    Send_VerifyCode,
    s_username_is_repititive,
    s_email_is_repititive,
    s_send_apply_For_Link,



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
struct CheckVerifySafe : headerPackage
{

    QString Link;
    QString Answer;

    CheckVerifySafe() {
        header = SAFE_VERIFY;
    }

    friend QDataStream &operator<<(QDataStream &out,const CheckVerifySafe &data)
       {
            out << static_cast<short>(data.header) << data.Link << data.Answer;
           return out;
       }

       friend QDataStream &operator>>(QDataStream &in, CheckVerifySafe &data)
       {
           short headerAsint;
           short msgAsshort;
           in >> headerAsint >> data.Link >> data.Answer;
           data.header = static_cast<HEADERS>(headerAsint);
           return in;
       }
};
struct AddUser_SPacket : headerPackage
{

    QByteArray data;
    AddUser_SPacket() {
        header = addUser_to_UsersDatabase;
    }
    friend QDataStream &operator<<(QDataStream &out,const AddUser_SPacket &data)
       {
            out << static_cast<short>(data.header) << data.data;
           return out;
       }

       friend QDataStream &operator>>(QDataStream &in, AddUser_SPacket &data)
       {
           short headerAsint;
           short msgAsshort;
           in >> headerAsint >> data.data;
           data.header = static_cast<HEADERS>(headerAsint);
           return in;
       }
};

#endif // PACKAGES_H
