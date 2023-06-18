#include "databasecontroller.h"

DataBaseController::DataBaseController()
{
    dataBase = QSqlDatabase::addDatabase("QSQLITE");
    dataBase.setDatabaseName("/home/sergey/qt_projects/client_server/server/data/massenger_db.db");
    if(dataBase.open())
    {
        qDebug() << "Sucsessful connect to db";
        query =  new QSqlQuery(dataBase);
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

void DataBaseController::InsertChat(int id_user1, int id_user2, QString path)
{
    query->prepare("INSERT INTO chat (id_user1, id_user2, masseges_path) VALUES (:id_user1, :id_user2, :masseges_path)");
    query->bindValue(":id_user1", id_user1);
    query->bindValue(":id_user2", id_user2);
    query->bindValue("masseges_path", path);
    query->exec();
    qDebug() << "DB error: " << query->lastError();
    query->clear();
}
