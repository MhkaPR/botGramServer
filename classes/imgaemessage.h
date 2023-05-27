#ifndef IMGAEMESSAGE_H
#define IMGAEMESSAGE_H

#include <QObject>

#include "package.h"
#include "textmessage.h"

class imgaeMessage : public package
{
public:
    imgaeMessage();

    virtual void deserialize(QByteArray buffer) override;
    virtual QByteArray serialize() override;

private:


    QChar typepack;
    QString name;
    quint32 count=0;
    quint64 size=0;
    QByteArray Data;
    TextMessage Info;



};

#endif // IMGAEMESSAGE_H
