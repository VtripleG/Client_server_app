#ifndef USER_H
#define USER_H
#include <QTcpSocket>


class User
{
public:
    QString Name;
    QTcpSocket * Socket;

    User(QString Name, QTcpSocket * Socket);
};

#endif // USER_H
