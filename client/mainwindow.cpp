#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
{
    size_block = 0;
    m_ui->setupUi(this);
    socket = new QTcpSocket(this);
    m_ui->stackedWidget->setCurrentIndex(1);
    connect(m_ui->lineEdit, &QLineEdit::returnPressed, this, &MainWindow::on_Send_button_clicked);
    connect(m_ui->lineEditSelect, &QLineEdit::returnPressed, this, &MainWindow::on_Select_button_clicked);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    connect(m_ui->names_list, &QListWidget::itemClicked, this, &MainWindow::slotSelectListItem);
    connect(m_ui->chatSpace, &QListWidget::itemClicked, this, &MainWindow::slotSelectResendMassege);
    connect(m_ui->edit_lineEdit, &QLineEdit::returnPressed, this, &MainWindow::on_send_edit_button_clicked);
    connect(m_ui->chatSpace, &QListWidget::itemDoubleClicked, this, &MainWindow::on_resend_button_clicked);
    connect(m_ui->lineEditUserName, &QLineEdit::returnPressed, this, &MainWindow::on_Connect_button_clicked);
    connect(m_ui->widget, &Graffiti_space::clicked, this, &MainWindow::ImageChanched);
    setCentralWidget(m_ui->stackedWidget);
}

MainWindow::~MainWindow()
{
    delete m_ui;
}


void MainWindow::on_Connect_button_clicked()
{
    Data.clear();
    socket->connectToHost("127.0.0.1", 2323);
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_4);
    self_name = m_ui->lineEditUserName->text();
    m_ui->lineEditUserName->clearFocus();
    out << self_name;
    socket->write(Data);
    m_ui->UserNameLabel->setText(self_name);
    m_ui->stackedWidget->setCurrentIndex(0);
    m_ui->stackedWidget_2->setCurrentIndex(0);
}

void MainWindow::on_Select_button_clicked()
{
    geter_name = m_ui->lineEditSelect->text();
    if(!sender_names.isEmpty())
    {
        bool flag = false;
        for(auto &item: sender_names)
        {
            if(item == geter_name)
                flag = true;
        }
        if(!flag)
        {
            sender_names.push_back(geter_name);
            m_ui->names_list->addItem(geter_name);
            chats.push_back(Chat(self_name, geter_name));
            graffitis.push_back(QImage());
            m_ui->names_list->setCurrentRow(chats.size()-1);
            SendOnChatSpace(chats[m_ui->names_list->currentRow()].getMasseges());
        }
    }
    else
    {
        sender_names.push_back(geter_name);
        m_ui->names_list->addItem(geter_name);
        chats.push_back(Chat(self_name, geter_name));
        graffitis.push_back(QImage());
        m_ui->names_list->setCurrentRow(chats.size()-1);
        SendOnChatSpace(chats[m_ui->names_list->currentRow()].getMasseges());
    }
    m_ui->lineEditSelect->clear();
    m_ui->lineEdit->setFocus();

}

void MainWindow::on_Send_button_clicked()
{
    if(m_ui->lineEdit->text() != "\0")
    {
        SendToServer(text_, NULL, m_ui->lineEdit->text());
        chats[m_ui->names_list->currentRow()].addMassege(self_name, m_ui->lineEdit->text(), false);
        SendOnChatSpace(chats[m_ui->names_list->currentRow()].getMasseges());
    }
    m_ui->lineEdit->clear();
}

void MainWindow::on_resend_button_clicked()
{
    int row_index;
    row_index = m_ui->chatSpace->currentRow();
    if(!chats[m_ui->names_list->currentRow()].getMasseges()[row_index].getImageFlag())
    {
        SendToServer(text_, NULL, chats[m_ui->names_list->currentRow()].resendMassege(row_index));
        chats[m_ui->names_list->currentRow()].addMassege(self_name, chats[m_ui->names_list->currentRow()].resendMassege(row_index), false);
    }
    else
    {
        SendToServer(image_, NULL, qCompress(chats[m_ui->names_list->currentRow()].getMasseges()[row_index].getText().toLocal8Bit(), 9).toBase64());
        chats[m_ui->names_list->currentRow()].addMassege(self_name, chats[m_ui->names_list->currentRow()].getMasseges()[row_index].getText(), true);
    }
    SendOnChatSpace(chats[m_ui->names_list->currentRow()].getMasseges());
    m_ui->resend_button->setEnabled(false);
    m_ui->delete_button->setEnabled(false);
    m_ui->edit_button->setEnabled(false);
}

void MainWindow::on_delete_button_clicked()
{
    int current_row = m_ui->chatSpace->currentRow();
    qDebug() << current_row;
    SendToServer(delete_, current_row, nullptr);
    chats[m_ui->names_list->currentRow()].deleteMassege(current_row);
    SendOnChatSpace(chats[m_ui->names_list->currentRow()].getMasseges());
    m_ui->resend_button->setEnabled(false);
    m_ui->delete_button->setEnabled(false);
    m_ui->edit_button->setEnabled(false);
}

void MainWindow::on_edit_button_clicked()
{
    m_ui->stackedWidget_2->setCurrentIndex(1);
    m_ui->edit_lineEdit->setText(chats[m_ui->names_list->currentRow()].getMasseges()[m_ui->chatSpace->currentRow()].getText());
    m_ui->resend_button->setEnabled(false);
    m_ui->delete_button->setEnabled(false);
    m_ui->edit_button->setEnabled(false);
}

void MainWindow::on_send_edit_button_clicked()
{
    int current_row;
    current_row = m_ui->chatSpace->currentRow();
    chats[m_ui->names_list->currentRow()].editMassege(m_ui->chatSpace->currentRow(), m_ui->edit_lineEdit->text());
    SendOnChatSpace(chats[m_ui->names_list->currentRow()].getMasseges());
    SendToServer(edit_, current_row, m_ui->edit_lineEdit->text());
    m_ui->edit_lineEdit->clear();
    m_ui->stackedWidget_2->setCurrentIndex(0);
    m_ui->lineEdit->setFocus();
}

void MainWindow::on_send_image_clicked()
{
    QString path;
    path = QFileDialog::getOpenFileName(this, "Select image", "/home/sergey/qt_projects", "PNG Image (*.png)");
    QImage image(path);
    QByteArray byte_ar;
    QBuffer cod_buff (&byte_ar);
    image.save(&cod_buff, "png");
    QString string = byte_ar.toBase64();
    chats[m_ui->names_list->currentRow()].addMassege(self_name, string, true);
    string = QString(qCompress(string.toLocal8Bit(), 9).toBase64());
    SendToServer(image_, NULL, string);
    SendOnChatSpace(chats[m_ui->names_list->currentRow()].getMasseges());
}

void MainWindow::on_graffiti_button_clicked()
{
    m_ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_send_graffiti_button_clicked()
{
    stream_flag = false;
    QString string = m_ui->widget->getImage();
    chats[m_ui->names_list->currentRow()].addMassege(self_name, string, true);
    string = QString(qCompress(string.toLocal8Bit(), 9).toBase64());
    SendToServer(image_, NULL, string);
    SendOnChatSpace(chats[m_ui->names_list->currentRow()].getMasseges());
    m_ui->widget->clearImage();
    m_ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_set_red_button_clicked()
{
    m_ui->widget->setColor(QColor(Qt::red));
}

void MainWindow::on_set_green_button_clicked()
{
    m_ui->widget->setColor(QColor(Qt::green));
}

void MainWindow::on_set_blue_button_clicked()
{
    m_ui->widget->setColor(QColor(Qt::blue));
}

void MainWindow::on_setBackGround_clicked()
{
    QString path;
    path = QFileDialog::getOpenFileName(this, "Select image", "/home/sergey/qt_projects", "PNG Image (*.png)");
    QImage image(path);
    m_ui->widget->setBackground(image);
}

void MainWindow::on_view_stream_clicked()
{
    m_ui->stackedWidget->setCurrentIndex(2);
    stream_flag = true;
    while(stream_flag = true)
    {
        if(stream_flag = false)
            break;
        m_ui->widget->setImage(graffitis[m_ui->names_list->currentRow()]);
    }
}

void MainWindow::on_stream_flag_clicked()
{
    stream_flag = true;
    while(stream_flag = true)
    {
        if(stream_flag = false)
            break;
        SendToServer(stream_graffiti_, NULL, m_ui->widget->getImage());
    }
}

void MainWindow::on_stream_flag_released()
{
    stream_flag = false;
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
    out << qint64(0) << action_flag << geter_name << send_string << row_index;
    out.device()->seek(0);
    out << qint64(Data.size() - sizeof(qint64));
    socket->write(Data);
}

void MainWindow::SendOnChatSpace(QList <Massege> massege)
{
    m_ui->chatSpace->clear();
    for ( auto &item: massege)
    {
        auto add_item = new QListWidgetItem;
        if (item.getName() == self_name)
        {
            add_item->setTextAlignment(Qt::AlignRight);
            if(item.getImageFlag() == false)
            {
                add_item->setText(item.getText());
                m_ui->chatSpace->addItem(add_item);
            }
            else
            {
                auto label = PrintImage(item.getText());
                label->setAlignment(Qt::AlignRight);
                m_ui->chatSpace->addItem(add_item);
                add_item->setSizeHint(QSize(label->pixmap().width(), label->pixmap().height()+10));
                m_ui->chatSpace->setItemWidget(add_item, label);
                qDebug() << label->width() << " " << label->height();
                qDebug() << label->pixmap().height();
                qDebug() << add_item->sizeHint();
            }
        }
        else
        {
            add_item->setTextAlignment(Qt::AlignLeft);
            if(item.getImageFlag() == false)
            {
                add_item->setText(item.getText());
                m_ui->chatSpace->addItem(add_item);
            }
            else
            {
                auto label = PrintImage(item.getText());
                label->setAlignment(Qt::AlignLeft);
                m_ui->chatSpace->addItem(add_item);
                add_item->setSizeHint(QSize(label->pixmap().width(), label->pixmap().height()+10));
                m_ui->chatSpace->setItemWidget(add_item, label);
                qDebug() << label->width() << " " << label->height();
                qDebug() << label->pixmap().height();
                qDebug() << add_item->sizeHint();
            }
        }
    }
}

QLabel *MainWindow::PrintImage(QString string)
{
    qDebug() << "image";
    QByteArray byte_array;
    byte_array = string.toLocal8Bit();
    byte_array = QByteArray::fromBase64(byte_array);
    QImage image = QImage::fromData(byte_array);
    auto label = new QLabel(this);
    if(image.height() > image.width())
        label->setPixmap(QPixmap::fromImage(image).scaled(image.width()/(image.height()/300), image.height()/(image.height()/300), Qt::KeepAspectRatio));
    else
        label->setPixmap(QPixmap::fromImage(image).scaled(image.width()/(image.width()/300), image.height()/(image.width()/300), Qt::KeepAspectRatio));
    label->setMask(QPixmap::fromImage(image).mask());
    return label;

}

void MainWindow::ReadMassege(QString sender_name, QString read_string, bool image_flag)
{
    if(!sender_names.isEmpty())
    {
        bool flag = false;
        for( auto &item: sender_names)
        {
            if(item == sender_name)
                flag = true;
        }
        if(!flag)
        {
            sender_names.push_back(sender_name);
            m_ui->names_list->addItem(sender_name);
            chats.push_back(Chat(self_name, sender_name));
            graffitis.push_back(QImage());
            chats[chats.size()-1].addMassege(sender_name, read_string, image_flag);
        }
        else
        {
            for(int i = 0; i < sender_names.size(); i++)
            {
                if(sender_name == chats[i].getSecomdName())
                {
                    chats[i].addMassege(sender_name, read_string, image_flag);
                    if(m_ui->names_list->currentRow() == i)
                        SendOnChatSpace(chats[m_ui->names_list->currentRow()].getMasseges());
                }
            }
        }
    }
    else
    {
        sender_names.push_back(sender_name);
        m_ui->names_list->addItem(sender_name);
        chats.push_back(Chat(self_name, sender_name));
        graffitis.push_back(QImage());
        chats[chats.size()-1].addMassege(sender_name, read_string, image_flag);
    }
}

void MainWindow::DeleteMassege(int row_index, QString sender_name)
{
    qDebug() << row_index;
    if(!sender_names.isEmpty())
    {
        for(int i = 0; i < sender_names.size(); i++)
        {
            if(sender_name == chats[i].getSecomdName())
            {
                chats[i].deleteMassege(row_index);
                if(m_ui->names_list->currentRow() == i)
                    SendOnChatSpace(chats[m_ui->names_list->currentRow()].getMasseges());
            }
        }
    }
}

void MainWindow::EditMassege(int row_index, QString sender_name, QString read_string)
{
    if(!sender_names.isEmpty())
    {
        for(int i = 0; i < chats.size(); i++)
        {
            if(chats[i].getSecomdName()  == sender_name)
            {
                chats[i].editMassege(row_index, read_string);
                if(m_ui->names_list->currentRow() == i)
                {
                    SendOnChatSpace(chats[m_ui->names_list->currentRow()].getMasseges());
                }
                break;
            }
        }
    }
}

void MainWindow::GetStreamingGraffiti(QString sender_name, QString read_string)
{
    QByteArray byte_array;
    byte_array = read_string.toLocal8Bit();
    byte_array = QByteArray::fromBase64(byte_array);
    QImage image = QImage::fromData(byte_array);
    if(!sender_names.isEmpty())
    {
        for(int i = 0; i < sender_names.size(); i++)
        {
            if(sender_name == chats[i].getSecomdName())
            {
                graffitis[i] = image;
            }
        }
    }
}


void MainWindow::slotReadyRead()
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
            }
            if (size_block > socket->bytesAvailable())
                break;
            Data.clear();
            qDebug() << "Read...";
            QString read_string;
            QString sender_name;
            int action_flag;
            int row_index;
            in >> action_flag >> sender_name >> read_string >> row_index;
            size_block = 0;
            switch (action_flag)
            {
            case text_:
                ReadMassege(sender_name, read_string, false);
                break;
            case image_:
            {
                read_string = QString(qUncompress(QByteArray::fromBase64(read_string.toLocal8Bit())));
                ReadMassege(sender_name, read_string, true);
                break;
            }
            case delete_:
                DeleteMassege(row_index, sender_name);
                break;
            case edit_:
                EditMassege(row_index, sender_name, read_string);
                break;
            case stream_graffiti_:
                GetStreamingGraffiti(sender_name, read_string);
                break;
            default:
                break;
            }
            break;
        }
    }
}

void MainWindow::slotSelectListItem()
{
    geter_name = m_ui->names_list->currentItem()->text();
    m_ui->lineEditSelect->clear();
    m_ui->lineEdit->setFocus();
    SendOnChatSpace(chats[m_ui->names_list->currentRow()].getMasseges());
}

void MainWindow::slotSelectResendMassege()
{
    if(chats[m_ui->names_list->currentRow()].getMasseges()[m_ui->chatSpace->currentRow()].getName() == self_name)
    {
        m_ui->resend_button->setEnabled(true);
        m_ui->delete_button->setEnabled(true);
        m_ui->edit_button->setEnabled(true);
    }
    else{m_ui->resend_button->setEnabled(true);}
}

void MainWindow::ImageChanched()
{
    qDebug() << 1;
}
