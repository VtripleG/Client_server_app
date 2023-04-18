#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QTcpSocket>
#include <QVector>

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QTcpSocket *socket;
    QTcpSocket *buff_socket;

    QByteArray Data;
    int massege_flag = 0;
    QVector <QTcpSocket*> sockets;

    void SendToServer(QString send_string);

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void slotReadyRead();

private slots:
    void on_pushButton_clicked();

    void on_Send_button_clicked();

    void on_Connect_button_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
