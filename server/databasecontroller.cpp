#include "databasecontroller.h"

DataBaseController::DataBaseController()
{
    dataBase = QSqlDatabase::addDatabase("QSQLITE");
    dataBase.setDatabaseName("/home/sergey/qt_projects/client_server/server/data/massenger_db.db");
    if(dataBase.open())
        qDebug() << "Sucsessful";
}
