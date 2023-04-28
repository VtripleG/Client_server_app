#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QTcpSocket>
#include <QVector>
#include <QMainWindow>
#include <QStackedWidget>
#include "Chat.h"
#include "QCloseEvent"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QTcpSocket *socket;
    QString geter_name;
    QString self_name;

    QByteArray Data;
    int massege_flag = 0;
    QVector <QString> sender_names;
    QVector <Chat> chats;

    void closeEvent(QCloseEvent *event);


    void SendToServer(int action_flag, int row_index, QString send_string);
    void SendOnChatSpace(QList <QPair<QString, QString>> massege);

    void ReadMassege(QString sender_name, QString read_string);
    void DeleteMassege(int row_index, QString sender_name);
    void EditMassege(int row_index, QString sender_name, QString read_string);

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void slotReadyRead();
    void slotSelectListItem();
    void slotSelectResendMassege();
    void slotBlockButtons();

private slots:
    void on_Send_button_clicked();

    void on_Connect_button_clicked();

    void on_Select_button_clicked();

    void on_resend_button_clicked();

    void on_delete_button_clicked();

    void on_edit_button_clicked();

    void on_send_edit_button_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
