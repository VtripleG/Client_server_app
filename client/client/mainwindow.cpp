#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &MainWindow::on_Send_button_clicked);
    connect(ui->lineEditSelect, &QLineEdit::returnPressed, this, &MainWindow::on_Select_button_clicked);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    connect(ui->names_list, &QListWidget::itemClicked, this, &MainWindow::slotSelectListItem);
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SendToServer(QString send_string)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_4);
    out << geter_name << send_string;
    socket->write(Data);
}

void MainWindow::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_4);
    if(in.status()==QDataStream::Ok){
        Data.clear();
        qDebug() << "Read...";
        QString read_string;
        QString sender_name;
        in >> sender_name >> read_string;
        if(!sender_names.isEmpty())
        {
            bool flag = false;
            for( auto &item: sender_names){
                if(item == sender_name)
                    flag = true;
            }
            if(!flag)
            {
                sender_names.push_back(sender_name);
                ui->names_list->addItem(sender_name);
                chats.push_back(Chat(self_name, sender_name));
                chats[chats.size()-1].add_massege(sender_name, read_string);
            }
            else{
                for(int i = 0; i < sender_names.size(); i++)
                {
                    if(sender_name == chats[i].second_name){
                        chats[i].add_massege(sender_name, read_string);
                        if(ui->names_list->currentRow() == i){
                            ui->textBrowser->clear();
                            ui->textBrowser->setHtml(chats[ui->names_list->currentRow()].getChatText()->toHtml());
                        }
                    }
                }
            }
        }
        else{
            sender_names.push_back(sender_name);
            ui->names_list->addItem(sender_name);
            chats.push_back(Chat(self_name, sender_name));
            chats[chats.size()-1].add_massege(sender_name, read_string);
        }



    }
}

void MainWindow::slotSelectListItem()
{
    ui->textBrowser->clear();
    geter_name = ui->names_list->currentItem()->text();
    ui->lineEditSelect->clear();
    ui->lineEdit->setFocus();
    ui->textBrowser->clear();
    ui->textBrowser->setHtml(chats[ui->names_list->currentRow()].getChatText()->toHtml());
}

void MainWindow::on_Send_button_clicked()
{
    if(ui->lineEdit->text() != "\0"){
        SendToServer(ui->lineEdit->text());
        chats[ui->names_list->currentRow()].add_massege(self_name, ui->lineEdit->text());
        ui->textBrowser->clear();
        ui->textBrowser->setHtml(chats[ui->names_list->currentRow()].getChatText()->toHtml());
    }
    qDebug() << chats[ui->names_list->currentRow()].massege;
    ui->lineEdit->clear();

}

void MainWindow::on_Connect_button_clicked()
{
    Data.clear();
    socket->connectToHost("127.0.0.1", 2323);
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_4);
    self_name = ui->lineEditUserName->text();
    out << self_name;
    socket->write(Data);
}

void MainWindow::on_Select_button_clicked()
{
    geter_name = ui->lineEditSelect->text();
    if(!sender_names.isEmpty())
    {
        bool flag = false;
        for(auto &item: sender_names){
            if(item == geter_name)
                flag = true;
        }
        if(!flag)
        {
            sender_names.push_back(geter_name);
            ui->names_list->addItem(geter_name);
            chats.push_back(Chat(self_name, geter_name));
            ui->names_list->setCurrentRow(chats.size()-1);
            ui->textBrowser->clear();
            ui->textBrowser->setHtml(chats[ui->names_list->currentRow()].getChatText()->toHtml());
        }

    }
    else{
        sender_names.push_back(geter_name);
        ui->names_list->addItem(geter_name);
        chats.push_back(Chat(self_name, geter_name));
        ui->names_list->setCurrentRow(chats.size()-1);
        ui->textBrowser->clear();
        ui->textBrowser->setHtml(chats[ui->names_list->currentRow()].getChatText()->toHtml());

    }
    ui->lineEditSelect->clear();
    ui->lineEdit->setFocus();

}
