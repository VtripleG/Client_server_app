#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    socket = new QTcpSocket(this);
    socket->connectToHost("127.0.0.1", 2323);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    ui->setupUi(this);
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
        qDebug() << "Read...";
        QString read_string;
        in >> read_string;
        ui->textBrowser->insertPlainText(read_string + "\n");
    }
    else{
        qDebug() << "Readind error...";
    }
}


void MainWindow::on_pushButton_clicked()
{
    if(ui->lineEdit->text() != "\0"){
    ui->textBrowser->setAlignment(Qt::AlignRight);
    ui->textBrowser->insertPlainText(ui->lineEdit->text()+"\n");
    SendToServer(ui->lineEdit->text());
    ui->lineEdit->clear();
    }
}

void MainWindow::SendToServer(QString send_string)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_4);
    out << send_string;
    socket->write(Data);
}

