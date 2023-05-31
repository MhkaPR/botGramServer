#ifndef PACKAGE_H
#define PACKAGE_H

#include <QObject>
#include <QMap>
#include <QDataStream>
#include <QTcpSocket>

class package
{
public:
    enum HEADERS : short
    {
        CONNECT,
        VERIFY,
        SYSTEM,
        SAFE_VERIFY,
        ADDUSER_TO_USERS_DATABASE,
        TOKENUSER,
        TEXTMESSAGE,
        ROOMS,
        UPDATE_CLIENT,
        FILEMESSAGE

    };

    enum SEND_STATE : short
    {
        sendMode,
        recieveMode,
        readedMode

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
    enum RoomType :short
    {
        PV,CHANELL,GROUPE,BOT
    };

    package();
    static QByteArray Packeting(HEADERS headerPacket, QByteArray bufferPacket);
    static HEADERS unPacketing(QByteArray& bufferPacket);
    HEADERS getheader();

protected:

    HEADERS header;
    virtual void deserialize(QByteArray buffer) = 0;
    virtual QByteArray serialize() =0;





};

#endif // PACKAGE_H
