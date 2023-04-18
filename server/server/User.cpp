#include "User.h"
#include <QTcpSocket>

User::User(QString Name, QTcpSocket * Socket)
{
    this->Name = Name;
    this->Socket = Socket;
}
