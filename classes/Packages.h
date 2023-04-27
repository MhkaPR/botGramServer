#ifndef PACKAGES_H
#define PACKAGES_H
#include <QObject>
enum HEADERS
{
    VERIFY,

};

struct headerPackage
{

    HEADERS header;

};

struct loginPackage : headerPackage
{
    bool IsLogin;
    QString username,password,email;
    loginPackage() {
        header = VERIFY;
        IsLogin=true;
        email  ="";
    }
};
#endif // PACKAGES_H
