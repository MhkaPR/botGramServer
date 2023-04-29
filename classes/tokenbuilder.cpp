#include "tokenbuilder.h"

TokenBuilder::TokenBuilder(QString Db_name,QSqlDatabase Mydb):databaseName(Db_name),db(Mydb)
{

    short length = 8;
    QString pattern = QStringLiteral("^[a-zA-Z0-9!@#$%^&*()]{%1}$").arg(length);
    QRegularExpression regex(pattern);
    QString randomString;
    do {
        while (true) {
            for (int i = 0; i < length; ++i) {
                randomString.append(QChar::fromLatin1(QRandomGenerator::global()->bounded(48, 122)));
            }
            if (regex.match(randomString).hasMatch()) {
                break;
            }
            randomString.clear();
        }
    } while (!IsCorrectToken(randomString));
    Token = randomString;
}

QString TokenBuilder::token()
{
    return Token;
}
