#include "Server.h"
#include "QDebug"

Server::Server()
{
    size_block = 0;
    if(this->listen(QHostAddress::Any, 2323))
        qDebug() << "Sucssesful launche" ;
    else
    {
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
    if (auth_flag == 0)
    {
        QDataStream in(socket);
        in.setVersion(QDataStream::Qt_6_4);
        if(in.status()==QDataStream::Ok)
        {
            Data.clear();
            for(;;)
            {
                if(size_block == 0)
                {
                    if (socket->bytesAvailable() < 2)
                        break;
                    in >> size_block;
                    qDebug() << "BlockSize = " << size_block;
                }
                if (socket->bytesAvailable() < size_block)
                    break;
                Data.clear();
                qDebug() << "Read...";
                QString read_string;
                int action_flag;
                int row_index;
                in >> action_flag >> end_adress >> read_string >> row_index;

                qDebug() << action_flag;
                qDebug() << end_adress;
                qDebug() << read_string;
                qDebug() << row_index;

                size_block = 0;
                switch (action_flag)
                {
                case text_:
                    SendToClient(text_, NULL, read_string);
                    break;
                case image_:
                    SendToClient(image_, NULL, read_string);
                    break;
                case delete_:
                    SendToClient(delete_, row_index, nullptr);
                    break;
                case edit_:
                    SendToClient(edit_, row_index, read_string);
                    break;
                case delete_socket_:
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
                break;
            }
        }
        else
        {
            qDebug() << "Readind error...";
        }
    }
    else
        {
        QDataStream in(socket);
        in.setVersion(QDataStream::Qt_6_4);
        if(in.status()==QDataStream::Ok)
        {
            qDebug() << "Read AUTH...";
            QString read_string;
            in >> read_string;
            users.push_back(User(read_string, socket));
        }
        else
        {
            qDebug() << "Readind AUTH error...";
        }
        auth_flag = 0;
    }
}

void Server::SendToClient(int action_flag, int row_index, QString send_string)
{
    Data.clear();
    for(const auto &item: users)
    {
        if(item.Socket == socket)
            sender_user = item;
    }
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_4);
    out << qint64(0) << action_flag << sender_user.Name << send_string << nullptr;
    out.device()->seek(0);
    out << qint64(Data.size() - sizeof(qint64));
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
