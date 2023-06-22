#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>

class DataBaseController
{
public:
    DataBaseController();
    void InsertUser(QString username, QString password);
    void InsertChat(QString username1, QString username2);

    QString GetPath (QString username1, QString username2);
    bool GetOnlineStatus(QString username);

    void SetOnline(QString username);
    void SetOffline(QString username);

    bool Authorization(QString username, QString password);
private:
    QSqlDatabase dataBase;
    QSqlQuery * query;
};

#endif // DATABASECONTROLLER_H
