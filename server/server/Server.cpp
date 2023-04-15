#include "Server.h"
#include "QDebug"

Server::Server()
{
    if(this->listen(QHostAddress::Any, 2323))
        qDebug() << "Sucssesful connect" ;
    else{
        qDebug() << "Error while connection";
    }
}

void Server::SendToClient(QString send_string)
{
    Data.clear();
//    send_string+='!';
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_4);
    out << send_string;
    for (int i = 0; i < sockets.size(); i++)
        if(sockets[i] != socket)
            sockets[i]->write(Data);
//    socket->write(Data);


}

void Server::incomingConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    sockets.push_back(socket);

}

void Server::slotReadyRead()
{
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_4);
    if(in.status()==QDataStream::Ok){
        qDebug() << "Read...";
        QString read_string;
        in >> read_string;
        SendToClient(read_string);
    }
    else{
        qDebug() << "Readind error...";
    }

}
