#include "Server.h"
#include "QDebug"

Server::Server(qint16 port)
{
    size_block = 0;
    if(this->listen(QHostAddress::Any, 2323))
    {
        qDebug() << "Sucssesful launche" ;
        dataBase = DataBaseController();
    }
    else
    {
        qDebug() << "Error while launching";
    }
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
            in >> action_flag;
            switch (action_flag)
            {
            case text_:
            {
                in >> end_adress >> read_string >> row_index;
                SendToClient(text_, zero, read_string);
            }
                break;
            case image_:
            {
                in >> end_adress >> read_string >> row_index;
                SendToClient(image_, zero, read_string);
            }
                break;
            case delete_:
            {
                in >> end_adress >> read_string >> row_index;
                SendToClient(delete_, row_index, nullptr);
            }
                break;
            case edit_:
            {
                in >> end_adress >> read_string >> row_index;
                SendToClient(edit_, row_index, read_string);
            }
                break;
            case stream_graffiti_:
            {
                in >> end_adress >> read_string >> row_index;
                SendToClient(stream_graffiti_, zero, read_string);
            }
                break;
            case login_:
            {
                qDebug() << "Read login...";
                QString username;
                QString password;
                in >> username >> password;
                if(dataBase.Authorization(username, password))
                {
                    qDebug() << "Sucsessful login";
                    users.push_back(User(username, socket));
                    dataBase.SetOnline(username);
                }
                else
                    qDebug() << "Unsucsessfull login";
                break;
            }
            case exit_:
            {
                in >> end_adress >> read_string >> row_index;
                qDebug() << "Delete user";
                auto iter_users = users.cbegin();
                auto iter_sockets = sockets.cbegin();
                for (size_t count = 0; count < users.size(); count++)
                {
                    if(socket == users[count].Socket){
                        users.erase(iter_users);
                        sockets.erase(iter_sockets);
                        dataBase.SetOffline(users[count].Name);
                        break;
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
        size_block = 0;
    }
    else
    {
        qDebug() << "Readind error...";
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
    out << qint64(0) << action_flag << sender_user.Name << send_string << row_index;
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
