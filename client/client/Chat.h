#ifndef CHAT_H
#define CHAT_H
#include <QTextBrowser>


class Chat
{
//    Q_OBJECT
public:
    QString first_name;
    QString second_name;
    QList <QPair<QString, QString>> massege;
    void add_massege(QString user, QString text);
    QTextBrowser * getChatText();
    Chat(QString first_name, QString second_name);
};

#endif // CHAT_H
