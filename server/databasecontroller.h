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
private:
    QSqlDatabase dataBase;
    QSqlQuery * query;
    QSqlTableModel * userTable;
    QSqlTableModel * chatTable;
};

#endif // DATABASECONTROLLER_H
