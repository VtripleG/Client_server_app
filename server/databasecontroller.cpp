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
    else
    {
        qDebug() << "Unsucsessful connect to db";
    }

}

void DataBaseController::InsertUser(QString username, QString password)
{
    query->prepare("INSERT INTO user (username, password, online) "
              "VALUES (:username, :password, :online)");
    query->bindValue(":username", username);
    query->bindValue(":password", password);
    query->bindValue(":online", 1);
    query->exec();
    qDebug() << "DB error: " << query->lastError();
    query->clear();
}

void DataBaseController::InsertChat(QString username1, QString username2)
{
    QString path = username1 + username2 + ".json";
    query->prepare("INSERT INTO chat (username1, username2, masseges_path) VALUES (:username1, :username2, :masseges_path)");
    query->bindValue(":username1", username1);
    query->bindValue(":username2", username2);
    query->bindValue(":masseges_path", path);
    query->exec();
    qDebug() << "DB error: " << query->lastError();
    query->clear();
}

QString DataBaseController::GetPath(QString username1, QString username2)
{
    QString path;
    query->prepare("SELECT masseges_path FROM chat WHERE username1 = :username1 AND username2 = :username2 OR username1 = :username2 AND username2 = :username1");
    query->bindValue(":username1",username1);
    query->bindValue(":username2", username2);
    if(query->exec())
    {
        while(query->next())
        {
            path = query->value(0).toString();
        }
    }
    qDebug() << "DB error: " << query->lastError();
    query->clear();
    return path;
}
