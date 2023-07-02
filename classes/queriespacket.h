#ifndef QUERIESPACKET_H
#define QUERIESPACKET_H

#include <QObject>

#include <QFile>

class QueriesPacket
{
public:
    QueriesPacket();

    static QString getQuery(QString QueryName);
};

#endif // QUERIESPACKET_H
