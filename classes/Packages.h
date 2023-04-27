#ifndef PACKAGES_H
#define PACKAGES_H
#include <QObject>
enum HEADERS
{
    VERIFY,
    SYSTEM,

};
enum SysCodes
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
    bool IsLogin;
    QString username
    ,password
    ,email;

    loginPacket() {
        header = VERIFY;
        IsLogin=true;
        email  ="";
    }
};
struct systemMessagePacket : headerPackage
{

    SysCodes msg;

    systemMessagePacket() {
        header = SYSTEM;
    }
};
#endif // PACKAGES_H
