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
              "VALUES ('John')");
//    query->bindValue(":name", "John");
    qDebug() <<  query->exec();
//    query->bindValue(":name", "Nik");
//    query->exec();
//    query->bindValue(":name", "Ted");
//    query->exec();
}
