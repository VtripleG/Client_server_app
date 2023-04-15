#include <QCoreApplication>
#include "Server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server Serv;
    return a.exec();
}
