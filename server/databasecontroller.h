#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlQuery>

class DataBaseController
{
public:
    DataBaseController();
    void InsertUser(QString username, QString password);
    void InsertChat(QString username1, QString username2);
    QString GetPath (QString username1, QString username2);
private:
    QSqlDatabase dataBase;
    QSqlQuery * query;
    QSqlTableModel * userTable;
    QSqlTableModel * chatTable;
};

#endif // DATABASECONTROLLER_H
