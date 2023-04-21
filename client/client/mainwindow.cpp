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

void MainWindow::slotReadyRead()
{
    ui->textBrowser->setAlignment(Qt::AlignLeft);
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
            for(const auto &item: sender_names){
                if(item == sender_name)
                    flag = true;
            }
            if(!flag)
            {
                sender_names.push_back(sender_name);
                ui->names_list->addItem(sender_name);
            }

        }
        else{
            sender_names.push_back(sender_name);
            ui->names_list->addItem(sender_name);
        }
        ui->textBrowser->insertPlainText(sender_name + ": " + read_string + "\n");
    }
}

void MainWindow::slotSelectListItem()
{
    geter_name = ui->names_list->currentItem()->text();
    ui->lineEditSelect->clear();
    ui->lineEdit->setFocus();
//    ui->lineEditSelect->setText(ui->names_list->currentItem());
}



void MainWindow::SendToServer(QString send_string)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_4);
    out << geter_name << send_string;
    socket->write(Data);
}


void MainWindow::on_Send_button_clicked()
{
    if(ui->lineEdit->text() != "\0"){
    ui->textBrowser->setAlignment(Qt::AlignRight);
    ui->textBrowser->insertPlainText(ui->lineEdit->text()+"\n");
    SendToServer(ui->lineEdit->text());
    ui->lineEdit->clear();
    }
}


void MainWindow::on_Connect_button_clicked()
{
    Data.clear();
    socket->connectToHost("127.0.0.1", 2323);
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_4);
    out << ui->lineEditUserName->text();
    socket->write(Data);
}


void MainWindow::on_Select_button_clicked()
{
    geter_name = ui->lineEditSelect->text();
    if(!sender_names.isEmpty())
    {
        bool flag = false;
        for(const auto &item: sender_names){
            if(item == geter_name)
                flag = true;
        }
        if(!flag)
        {
            sender_names.push_back(geter_name);
            ui->names_list->addItem(geter_name);
        }

    }
    else{
        sender_names.push_back(geter_name);
        ui->names_list->addItem(geter_name);
    }
}

