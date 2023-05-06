#ifndef CHAT_H
#define CHAT_H
#include <QTextBrowser>
#include "Massege.h"


class Chat
{
//    Q_OBJECT
public:
    QString first_name;
    QString second_name;
    QList <Massege> massege;


    void delete_massege(int index);
    void edit_massege(int index, QString string);
    void add_massege(QString user, QString text, bool flag);
    QString resend_massege(int index);
//    QTextBrowser * getChatText();
    Chat(QString first_name, QString second_name);
};

#endif // CHAT_H
