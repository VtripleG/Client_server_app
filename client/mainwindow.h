#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QTcpSocket>
#include <QVector>
#include <QMainWindow>
#include <QStackedWidget>
#include <QCloseEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QImage>
#include <QBuffer>
#include <QFileDialog>
#include <QLabel>
#include <QGraphicsView>
#include "Graffiti_space.h"
#include "Chat.h"




QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    void closeEvent(QCloseEvent *event);

    void SendToServer(int action_flag, int row_index, QString send_string);
    void SendOnChatSpace(QList <Massege> massege);
    QLabel * PrintImage(QString string);

    void ReadMassege(QString sender_name, QString read_string, bool image_flag);
    void DeleteMassege(int row_index, QString sender_name);
    void EditMassege(int row_index, QString sender_name, QString read_string);
    void GetStreamingGraffiti(QString sender_name, QString read_string);

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void slotReadyRead();
    void slotSelectListItem();
    void slotSelectResendMassege();
    void ImageChanched();

private slots:
    void on_Send_button_clicked();

    void on_Connect_button_clicked();

    void on_Select_button_clicked();

    void on_resend_button_clicked();

    void on_delete_button_clicked();

    void on_edit_button_clicked();

    void on_send_edit_button_clicked();

    void on_send_image_clicked();

    void on_graffiti_button_clicked();

    void on_send_graffiti_button_clicked();

    void on_set_red_button_clicked();

    void on_set_green_button_clicked();

    void on_set_blue_button_clicked();

    void on_setBackGround_clicked();

    void on_view_stream_clicked();

    void on_stream_flag_clicked();

    void on_stream_flag_released();

private:
    Ui::MainWindow *m_ui;

    enum actiom_numbers : int
    {
        text_ = 1,
        image_ = 2,
        delete_ = 3,
        edit_ = 4,
        stream_graffiti_ = 5
    };

    QTcpSocket *socket;

    QString geter_name;
    QString self_name;

    QByteArray Data;

    int massege_flag = 0;
    qint64 size_block;

    bool stream_flag = false;

    QVector <QString> sender_names;
    QVector <QImage> graffitis;
    QVector <Chat> chats;
};
#endif // MAINWINDOW_H
