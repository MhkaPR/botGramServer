QT       += core gui
QT += network
QT += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

DESTDIR = $$PWD
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    classes/adduser_spacket.cpp \
    classes/authentication.cpp \
    classes/checkverifysafepacket.cpp \
    classes/client.cpp \
    classes/client_mssages.cpp \
    classes/connectverify.cpp \
    classes/database.cpp \
    classes/loginpacket.cpp \
    classes/package.cpp \
#    classes/package.cpp \
    classes/pvroom.cpp \
    classes/queriespacket.cpp \
    classes/room.cpp \
    classes/roompacket.cpp \
    classes/systemmessagepacket.cpp \
    classes/textmessage.cpp \
    classes/tokenbuilder.cpp \
    classes/tokenpacket.cpp \
    classes/updateclient.cpp \
    classes/verify.cpp \
    main.cpp \
    server.cpp \
#    verify.cpp

HEADERS += \
    classes/Packages.h \
    classes/adduser_spacket.h \
    classes/authentication.h \
    classes/checkverifysafepacket.h \
    classes/client.h \
    classes/client_mssages.h \
    classes/connectverify.h \
    classes/database.h \
    classes/loginpacket.h \
    classes/package.h \
#    classes/package.h \
    classes/pvroom.h \
    classes/queriespacket.h \
    classes/room.h \
    classes/roompacket.h \
    classes/systemmessagepacket.h \
    classes/textmessage.h \
    classes/tokenbuilder.h \
    classes/tokenpacket.h \
    classes/updateclient.h \
    classes/verify.h \
    server.h \
#    verify.h

FORMS += \
    server.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
