#include "Chat.h"

QString Chat::getFirstName()
{
    return m_first_name;
}

QString Chat::getSecomdName()
{
    return m_second_name;
}

QList<Massege> Chat::getMasseges()
{
    return m_masseges;
}

void Chat::setFirstName(QString first_name)
{
    m_first_name = first_name;
}

void Chat::setSecondName(QString second_name)
{
    m_second_name = second_name;
}

void Chat::setMasseges(QList <Massege> massege)
{
    m_masseges = massege;
}

void Chat::deleteMassege(int index)
{
    m_masseges.removeAt(index);
}

void Chat::editMassege(int index, QString string)
{
    m_masseges[index].setText(string);
}

void Chat::addMassege(QString user, QString text, bool flag)
{
    m_masseges.append(Massege(user, text, flag));
}

QString Chat::resendMassege(int index)
{
    QString re_massege = m_masseges[index].getName() + ": " + m_masseges[index].getText();
    return re_massege;
}

Chat::Chat(QString first_name, QString second_name)
{
    this->m_first_name = first_name;
    this->m_second_name = second_name;

}
