#ifndef SERVER_H
#define SERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include "User.h"


class Server: public QTcpServer
{
    Q_OBJECT
public:
    QTcpSocket *socket;
    QVector <QTcpSocket*> sockets;
    QVector <User> users;
    User sender_user;
    QString end_adress;
    int massege_flag = 0;
    int auth_flag;
    Server();
private:
    QByteArray Data;
    void SendToClient(QString send_string);
//    void SendSockets();
public slots:
    void slotAutorisation();
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();

};

#endif // SERVER_H
