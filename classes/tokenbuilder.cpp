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

bool TokenBuilder::IsCorrectToken(QString str)
{

    QSqlQuery Q(db);

    Q.prepare("SELECT Token FROM "+TableName+" WHERE Token = :t");
    Q.bindValue(":t",str);
    if(!Q.exec())
    {
        sendmessage(Q.lastError().text());
        Q.clear();
        db.close();
        exit(1);
    }
    if(Q.next()) return false;
    return true;
}
