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
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_4);
    out << send_string;
    for (int i = 0; i < users.size(); i++)
    {
        if(end_adress == users[i].Name)
            users[i].Socket->write(Data);
    }
//    socket->write(Data);


}

//void Server::SendSockets()
//{
//    Data.clear();
//    QDataStream out(&Data, QIODevice::WriteOnly);
//    out.setVersion(QDataStream::Qt_6_4);
//    massege_flag = 1; //1 - socket 2-текстовое сообщение
//    for(int i = 0; i < sockets.size(); i++)
//        for (int j = 0; j < sockets.size(); j++)
//        {
//        out << massege_flag << sockets[j];
//        sockets[i]->write(Data);
//        }

//}

void Server::slotAutorisation()
{
    auth_flag = 1;

}

void Server::incomingConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    connect(this, &QTcpServer::newConnection, this, &Server::slotAutorisation);
    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    sockets.push_back(socket);
}

void Server::slotReadyRead()
{
    if (auth_flag == 0){
        socket = (QTcpSocket*)sender();
        QDataStream in(socket);
        in.setVersion(QDataStream::Qt_6_4);
        if(in.status()==QDataStream::Ok){
            qDebug() << "Read...";
            QString read_string;
            in >> end_adress >> read_string;
            qDebug() << end_adress;
            qDebug() << read_string;
            SendToClient(read_string);
    }
        else{qDebug() << "Readind error...";
    }
    }
    else{
        socket = (QTcpSocket*)sender();
        QDataStream in(socket);
        in.setVersion(QDataStream::Qt_6_4);
        if(in.status()==QDataStream::Ok){
            qDebug() << "Read AUTH...";
            QString read_string;
            in >> read_string;
            users.push_back(User(read_string, socket));
            qDebug() << users[0].Name;
            qDebug() << users[0].Socket;
    }
        else{qDebug() << "Readind AUTH error...";}
        auth_flag = 0;

    }

}
