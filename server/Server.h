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
    Server();
private:
    QByteArray Data;
    void SendToClient(int action_flag, int row_index, QString send_string);
    void SendImageToClient(QString send_string);
    void SendEditToClient(int row_index, QString send_string);
    void SendDeleteToClient(int row_index);

    QTcpSocket *socket;
    QVector <QTcpSocket*> sockets;
    QVector <User> users;
    User sender_user;
    QString end_adress;
    int massege_flag = 0;
    int auth_flag;
    qint64 size_block;

    enum actiom_numbers : int
    {
        text_ = 1,
        image_ = 2,
        delete_ = 3,
        edit_ = 4,
        delete_socket_ = 9
    };
public slots:
    void slodDeleteSocket();
    void slotAutorisation();
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();

};

#endif // SERVER_H
