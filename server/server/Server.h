#ifndef SERVER_H
#define SERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>


class Server: public QTcpServer
{
    Q_OBJECT
public:
    QTcpSocket *socket;
    QVector <QTcpSocket*> sockets;
    Server();
private:
    QByteArray Data;
    void SendToClient(QString send_string);
public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();

};

#endif // SERVER_H
