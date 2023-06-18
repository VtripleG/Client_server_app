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
    void InsertUser(QString user_name);
    void InsertChat(int id_user1, int id_user2, QString path);
private:
    QSqlDatabase dataBase;
    QSqlQuery * query;
    QSqlTableModel * userTable;
    QSqlTableModel * chatTable;
};

#endif // DATABASECONTROLLER_H
