#ifndef CHAT_H
#define CHAT_H
#include <QTextBrowser>
#include "Massege.h"


class Chat
{
//    Q_OBJECT
private:
    QString m_first_name;
    QString m_second_name;
    QList <Massege> m_masseges;

public:
    QString getFirstName();
    QString getSecomdName();
    QList<Massege> getMasseges();

    void setFirstName(QString first_name);
    void setSecondName(QString second_name);
    void setMasseges(QList <Massege> massege);

    void deleteMassege(int index);
    void editMassege(int index, QString string);
    void addMassege(QString user, QString text, bool flag);
    QString resendMassege(int index);

    Chat(QString first_name, QString second_name);
};

#endif // CHAT_H
