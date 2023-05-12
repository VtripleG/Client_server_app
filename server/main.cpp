#include <QCoreApplication>
#include "Server.h"
#include "iostream"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qint16 port = a.arguments()[1].toInt();
    Server Serv(port);
    return a.exec();
}
