#include "databasecontroller.h"

DataBaseController::DataBaseController()
{
    dataBase = QSqlDatabase::addDatabase("QSQLITE");
    dataBase.setDatabaseName("/home/sergey/qt_projects/client_server/server/data/massenger_db.db");
    if(dataBase.open())
    {
        qDebug() << "Sucsessful connect to db";
        query =  new QSqlQuery(dataBase);
        InsertUser(" ");
    }
}

void DataBaseController::InsertUser(QString user_name)
{
    query->prepare("INSERT INTO user (name) "
              "VALUES (:name)");
    query->bindValue(":name", user_name);
    query->exec();
    qDebug() << "DB error: " << query->lastError();
    query->clear();
}
