#include "Server.h"
#include "QDebug"

Server::Server()
{
    if(this->listen(QHostAddress::Any, 2323))
        qDebug() << "Sucssesful launche" ;
    else{
        qDebug() << "Error while launching";
    }
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

void Server::slotAutorisation()
{
    auth_flag = 1;

}

void Server::slotReadyRead()
{
    socket = (QTcpSocket*)sender();
    if (auth_flag == 0){


        QDataStream in(socket);
        in.setVersion(QDataStream::Qt_6_4);
        if(in.status()==QDataStream::Ok){
            qDebug() << "Read...";
            QString read_string;
            int action_flag;
            int row_index;
            in >> action_flag >> end_adress >> read_string >> row_index;
            qDebug() << end_adress;
            qDebug() << read_string;
            switch (action_flag) {
            case 1:
                SendToClient(read_string);
                break;
            case 2:
                break;
            case 3:
                SendDeleteToClient(row_index);
                break;
            case 4:
                SendEditToClient(row_index, read_string);
                break;
            case 9:
            {
                qDebug() << "Delete user";
                auto iter_users = users.cbegin();
                auto iter_sockets = sockets.cbegin();
                for (size_t count = 0; count < users.size(); count++)
                {
                    if(socket == users[count].Socket){
                        users.erase(iter_users);
                        sockets.erase(iter_sockets);
                    }
                    iter_users++;
                    iter_sockets++;
                }
                break;
            }
            default:
                break;
            }

    }
        else{qDebug() << "Readind error...";
    }
    }
    else{
        QDataStream in(socket);
        in.setVersion(QDataStream::Qt_6_4);
        if(in.status()==QDataStream::Ok){
            qDebug() << "Read AUTH...";
            QString read_string;
            in >> read_string;
            users.push_back(User(read_string, socket));
    }
        else{qDebug() << "Readind AUTH error...";}
        auth_flag = 0;

    }

}

void Server::SendToClient(QString send_string)
{
    Data.clear();
    for(const auto &item: users)
    {
        if(item.Socket == socket)
            sender_user = item;
    }
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_4);
    out << 1 << sender_user.Name << send_string << nullptr;
    for (int i = 0; i < users.size(); i++)
    {
        if(end_adress == users[i].Name)
            users[i].Socket->write(Data);
    }
}

void Server::SendEditToClient(int row_index, QString send_string)
{
    Data.clear();
    for(const auto &item: users)
    {
        if(item.Socket == socket)
            sender_user = item;
    }
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_4);
    out << 4 << sender_user.Name << send_string << row_index;
    for (int i = 0; i < users.size(); i++)
    {
        if(end_adress == users[i].Name)
            users[i].Socket->write(Data);
    }

}

void Server::SendDeleteToClient(int row_index)
{
    Data.clear();
    for(const auto &item: users)
    {
        if(item.Socket == socket)
            sender_user = item;
    }
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_4);
    out << 3 << sender_user.Name << nullptr << row_index;
    for (int i = 0; i < users.size(); i++)
    {
        if(end_adress == users[i].Name)
            users[i].Socket->write(Data);
    }
}

void Server::slodDeleteSocket()
{
    qDebug() << "Delete socket";
    auto iter = users.cbegin();
    for (size_t i = 0; i < users.size(); i++)
    {
        if(users[i].Socket == socket)
        {
            socket->deleteLater();
            users.erase(iter);
            break;
        }
        iter++;
    }

}
