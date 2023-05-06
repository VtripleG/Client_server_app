#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    ui->stackedWidget->setCurrentIndex(1);
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &MainWindow::on_Send_button_clicked);
    connect(ui->lineEditSelect, &QLineEdit::returnPressed, this, &MainWindow::on_Select_button_clicked);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    connect(ui->names_list, &QListWidget::itemClicked, this, &MainWindow::slotSelectListItem);
    connect(ui->chatSpace, &QListWidget::itemClicked, this, &MainWindow::slotSelectResendMassege);
    connect(ui->edit_lineEdit, &QLineEdit::returnPressed, this, &MainWindow::on_send_edit_button_clicked);
//    connect(ui->chatSpace, &QListWidget::itemSelectionChanged, this, &MainWindow::slotBlockButtons);
    connect(ui->chatSpace, &QListWidget::itemDoubleClicked, this, &MainWindow::on_resend_button_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
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
    ui->stackedWidget->setCurrentIndex(0);
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
            SendOnChatSpace(chats[ui->names_list->currentRow()].massege);

        }

    }
    else{
        sender_names.push_back(geter_name);
        ui->names_list->addItem(geter_name);
        chats.push_back(Chat(self_name, geter_name));
        ui->names_list->setCurrentRow(chats.size()-1);
        SendOnChatSpace(chats[ui->names_list->currentRow()].massege);


    }
    ui->lineEditSelect->clear();
    ui->lineEdit->setFocus();

}

void MainWindow::on_Send_button_clicked()
{
    if(ui->lineEdit->text() != "\0"){
        SendToServer(1, NULL, ui->lineEdit->text());
        chats[ui->names_list->currentRow()].add_massege(self_name, ui->lineEdit->text(), false);
        SendOnChatSpace(chats[ui->names_list->currentRow()].massege);

    }
    ui->lineEdit->clear();

}

void MainWindow::on_resend_button_clicked()
{
    int row_index;
    row_index = ui->chatSpace->currentRow();
    SendToServer(1, NULL, chats[ui->names_list->currentRow()].resend_massege(row_index));
    chats[ui->names_list->currentRow()].add_massege(self_name, chats[ui->names_list->currentRow()].resend_massege(row_index), false);
    SendOnChatSpace(chats[ui->names_list->currentRow()].massege);

    qDebug() << chats[ui->names_list->currentRow()].resend_massege(row_index);
    ui->resend_button->setEnabled(false);
    ui->delete_button->setEnabled(false);
    ui->edit_button->setEnabled(false);
}

void MainWindow::on_delete_button_clicked()
{
    int current_row = ui->chatSpace->currentRow();
    SendToServer(3, current_row, nullptr);
    chats[ui->names_list->currentRow()].delete_massege(current_row);
    SendOnChatSpace(chats[ui->names_list->currentRow()].massege);
    ui->resend_button->setEnabled(false);
    ui->delete_button->setEnabled(false);
    ui->edit_button->setEnabled(false);
}

void MainWindow::on_edit_button_clicked()
{
    ui->edit_lineEdit->raise();
    ui->send_edit_button->raise();
    ui->edit_lineEdit->setText(chats[ui->names_list->currentRow()].massege[ui->chatSpace->currentRow()].massege_string);
    ui->resend_button->setEnabled(false);
    ui->delete_button->setEnabled(false);
    ui->edit_button->setEnabled(false);
}

void MainWindow::on_send_edit_button_clicked()
{
    int current_row;
    current_row = ui->chatSpace->currentRow();
    chats[ui->names_list->currentRow()].edit_massege(ui->chatSpace->currentRow(), ui->edit_lineEdit->text());
    SendOnChatSpace(chats[ui->names_list->currentRow()].massege);
    SendToServer(4, current_row, ui->edit_lineEdit->text());

    ui->edit_lineEdit->clear();
    ui->Send_button->raise();
    ui->lineEdit->raise();
    ui->lineEdit->setFocus();
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    SendToServer(9, NULL, nullptr);
}

void MainWindow::SendToServer(int action_flag, int row_index, QString send_string)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_4);
    out << action_flag << geter_name << send_string << row_index;
    socket->write(Data);
}

void MainWindow::SendOnChatSpace(QList <Massege> massege)
{
    ui->chatSpace->clear();
    for ( auto &item: massege)
    {
        auto add_item = new QListWidgetItem;

        if (item.name == self_name)
        {
            add_item->setTextAlignment(Qt::AlignRight);
            if(item.image_flag == false)
            {
                add_item->setText(item.massege_string);
                ui->chatSpace->addItem(add_item);
            }
            else
            {
                ui->chatSpace->setIconSize(QSize(225, 400));
                qDebug() << "image";
                QByteArray byte_array;
                QString buff_string;
                buff_string = item.massege_string;
                byte_array = buff_string.toLocal8Bit();
                byte_array = QByteArray::fromBase64(byte_array);
                QImage image = QImage::fromData(byte_array);
                add_item->setIcon(QPixmap::fromImage(image));
                ui->chatSpace->addItem(add_item);
            }
        }
        else{
            add_item->setTextAlignment(Qt::AlignLeft);
            if(item.image_flag == false)
            {
                add_item->setText(item.massege_string);
                ui->chatSpace->addItem(add_item);
            }
            else
            {
                ui->chatSpace->setIconSize(QSize(225, 400));
                qDebug() << "image";
                QByteArray byte_array;
                QString buff_string;
                buff_string = item.massege_string;
                byte_array = buff_string.toLocal8Bit();
                byte_array = QByteArray::fromBase64(byte_array);
                QImage image = QImage::fromData(byte_array);
                add_item->setIcon(QPixmap::fromImage(image));
                ui->chatSpace->addItem(add_item);
            }

        }
    }
}

void MainWindow::ReadMassege(QString sender_name, QString read_string, bool image_flag)
{
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
            chats[chats.size()-1].add_massege(sender_name, read_string, image_flag);
        }
        else{
            for(int i = 0; i < sender_names.size(); i++)
            {
                if(sender_name == chats[i].second_name){
                    chats[i].add_massege(sender_name, read_string, image_flag);
                    if(ui->names_list->currentRow() == i){
                        SendOnChatSpace(chats[ui->names_list->currentRow()].massege);
                    }
                }
            }
        }
    }
    else{
        sender_names.push_back(sender_name);
        ui->names_list->addItem(sender_name);
        chats.push_back(Chat(self_name, sender_name));
        chats[chats.size()-1].add_massege(sender_name, read_string, image_flag);
    }

}

void MainWindow::DeleteMassege(int row_index, QString sender_name)
{
    if(!sender_names.isEmpty()){
        for(int i = 0; i < sender_names.size(); i++)
        {
            if(sender_name == chats[i].second_name){
                chats[i].delete_massege(row_index);
                if(ui->names_list->currentRow() == i){
                    SendOnChatSpace(chats[ui->names_list->currentRow()].massege);
                }
            }
        }
    }

}

void MainWindow::EditMassege(int row_index, QString sender_name, QString read_string)
{
    if(!sender_names.isEmpty()){
        for(int i = 0; i < chats.size(); i++)
        {
            if(chats[i].second_name  == sender_name){
                chats[i].edit_massege(row_index, read_string);
                if(ui->names_list->currentRow() == i){
                    SendOnChatSpace(chats[ui->names_list->currentRow()].massege);
                }
                break;
            }
        }
    }

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
        int action_flag;
        int row_index;
        in >> action_flag >> sender_name >> read_string >> row_index;
        switch (action_flag) {
        case 1:
            ReadMassege(sender_name, read_string, false);
            break;
        case 2:
            ReadMassege(sender_name, read_string, true);
            break;
        case 3:
            DeleteMassege(row_index, sender_name);
            break;
        case 4:
            EditMassege(row_index, sender_name, read_string);
            break;
        default:
            break;
        }






    }
}

void MainWindow::slotSelectListItem()
{
    geter_name = ui->names_list->currentItem()->text();
    ui->lineEditSelect->clear();
    ui->lineEdit->setFocus();
    SendOnChatSpace(chats[ui->names_list->currentRow()].massege);

}

void MainWindow::slotSelectResendMassege()
{
    if(chats[ui->names_list->currentRow()].massege[ui->chatSpace->currentRow()].name == self_name){
    ui->resend_button->setEnabled(true);
    ui->delete_button->setEnabled(true);
    ui->edit_button->setEnabled(true);
    }
    else{ui->resend_button->setEnabled(true);}
}

void MainWindow::slotBlockButtons()
{
//    if(!ui->chatSpace->hasFocus())
//    {
//        ui->resend_button->setEnabled(false);
//        ui->delete_button->setEnabled(false);
//        ui->edit_button->setEnabled(false);
//    }

    ui->resend_button->setEnabled(false);
    ui->delete_button->setEnabled(false);
    ui->edit_button->setEnabled(false);
}







void MainWindow::on_send_image_clicked()
{
    QString path;
    path = QFileDialog::getOpenFileName(this, "Select image", "/home/sergey", "PNG Image (*.png)");
    QImage image(path);
    QByteArray byte_ar;
    QBuffer cod_buff (&byte_ar);
    image.save(&cod_buff, "png");
    QString string = byte_ar.toBase64();

    chats[ui->names_list->currentRow()].add_massege(self_name, string, true);

    SendToServer(2, NULL, string);
    SendOnChatSpace(chats[ui->names_list->currentRow()].massege);

}

