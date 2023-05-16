#include "database.h"

DataBase::DataBase()
{

}

void DataBase::sendmessage(QString str)
{
    QMessageBox *msg = new QMessageBox;
    msg->setText(str);
    msg->exec();
    delete msg;
}

bool DataBase::IsColumnInTable(QString ColName, QString tableName)
{


    QStringList splitedData = ColName.split("_");

    if(splitedData[0]== "pv")
    {
    splitedData.swapItemsAt(1,2);
    QString swaped = splitedData.join("_");

    QSqlQuery query_IsCulomnInTable(db);

    query_IsCulomnInTable.prepare(
                "SELECT COUNT(*) FROM pragma_table_info(:t) WHERE name = :colName OR name = :swaped;"
                );
    query_IsCulomnInTable.bindValue(":t",tableName);
    query_IsCulomnInTable.bindValue(":colName",ColName);
    query_IsCulomnInTable.bindValue(":swaped",swaped);
    if(!query_IsCulomnInTable.exec())
    {
        sendmessage("IsColumnInTable: "+query_IsCulomnInTable.lastError().text());
        query_IsCulomnInTable.clear();
        exit(1);

    }
    query_IsCulomnInTable.first();
    bool ans = query_IsCulomnInTable.value(0).toBool();
    if(!ans)
    query_IsCulomnInTable.finish();
    return ans;
    }
    else
    {
        QSqlQuery query_IsCulomnInTable(db);

        query_IsCulomnInTable.prepare(
                    "SELECT COUNT(*) FROM pragma_table_info(:t) WHERE name = :colName ;"
                    );
        query_IsCulomnInTable.bindValue(":t",tableName);
        query_IsCulomnInTable.bindValue(":colName",ColName);
        if(!query_IsCulomnInTable.exec())
        {
            sendmessage("IsColumnInTable: "+query_IsCulomnInTable.lastError().text());
            query_IsCulomnInTable.clear();
            exit(1);

        }
        query_IsCulomnInTable.first();
        bool ans = query_IsCulomnInTable.value(0).toBool();
        query_IsCulomnInTable.finish();
        return ans;
    }

}

short DataBase::add_columnInTable(QString ColName, QString tableName)
{
    QSqlQuery query_add_columnInTable(db);

    query_add_columnInTable.prepare(
                "ALTER TABLE "+tableName+" ADD COLUMN "+ColName+" TEXT DEFAULT NULL;"
                );

    if(!query_add_columnInTable.exec())
    {
        sendmessage("query_add_columnInTable:"+query_add_columnInTable.lastError().text());
        query_add_columnInTable.clear();
        return DATABASE_ERROR;

    }
    query_add_columnInTable.finish();
    return COLUMN_SUCCESSFULLY_ADDED;
}

short DataBase::create_Table(QString tableName, const QMap<QString, QString> ColInfo)
{

    QSqlQuery query_create_Table(db);

    query_create_Table.prepare(
                "CREATE TABLE IF NOT EXISTS "+tableName+" ( "
                                                        "ID INTEGER "
                                                        ");"
                );

    if(!query_create_Table.exec())
    {
        sendmessage("create_Table : "+query_create_Table.lastError().text());
        query_create_Table.clear();
        return  DATABASE_ERROR;

    }
    else
    {
        query_create_Table.clear();
        //--------------------------
        foreach ( QString key, ColInfo.keys())
        {
            query_create_Table.prepare(
                        "ALTER TABLE "+tableName+" ADD COLUMN "+key+" "+ColInfo.value(key)+";"
                        );
            if(!query_create_Table.exec())
            {
                sendmessage("create_Table \\ add column : "+query_create_Table.lastError().text());
                query_create_Table.clear();
                return  DATABASE_ERROR;

            }
            query_create_Table.clear();
        }

    }

    query_create_Table.finish();
    return  ADDED_TABLE;
}

