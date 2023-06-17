#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlTableModel>

class DataBaseController
{
public:
    DataBaseController();
private:
    QSqlDatabase dataBase;
    QSqlTableModel * userTable;
    QSqlTableModel * chatTable;
};

#endif // DATABASECONTROLLER_H
