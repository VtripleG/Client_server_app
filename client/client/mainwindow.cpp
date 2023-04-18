#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &MainWindow::on_Send_button_clicked);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

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
        in >> read_string;
        ui->textBrowser->insertPlainText(read_string + "\n");
}
}



void MainWindow::SendToServer(QString send_string)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_4);
    out << ui->lineEditSelect->text() << send_string;
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
//    socket->connectToHost("127.0.0.1", 2323);

}

