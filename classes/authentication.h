#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <QObject>
#include <QRandomGenerator>
#include "database.h"
#include "Packages.h"

class Authentication : public DataBase
{
public:
    Authentication();
    CheckVerifySafePacket getSafeVerify();

private:
    CheckVerifySafePacket safeVerify;
};

#endif // AUTHENTICATION_H
