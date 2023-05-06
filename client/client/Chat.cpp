#include "Chat.h"

void Chat::delete_massege(int index)
{
    massege.removeAt(index);
}

void Chat::edit_massege(int index, QString string)
{
    massege[index].massege_string = string;
}

void Chat::add_massege(QString user, QString text, bool flag)
{
    massege.append(Massege(user, text, flag));
}

QString Chat::resend_massege(int index)
{
    QString re_massege = massege[index].name + ": " + massege[index].massege_string;
    return re_massege;
}

//QTextBrowser* Chat::getChatText()
//{
//    auto textBrowser = new QTextBrowser;
//    for ( auto &item: massege)
//    {
//        if (item.first == first_name)
//        {
//            textBrowser->setAlignment(Qt::AlignRight);
//            textBrowser->insertPlainText(item.second.first + "\n");
//        }
//        else{
//            textBrowser->setAlignment(Qt::AlignLeft);
//            textBrowser->insertPlainText(item.second.first + "\n");

//        }
//    }
//    return textBrowser;
//}

Chat::Chat(QString first_name, QString second_name)
{
    this->first_name = first_name;
    this->second_name = second_name;

}
