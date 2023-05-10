#ifndef MASSEGE_H
#define MASSEGE_H
#include <QString>


class Massege
{
public:
    Massege(QString name, QString massege_string, bool image_flag);

    QString getName();
    QString getText();
    bool getImageFlag();

    void setName(QString name);
    void setText(QString text_massege);
    void setImageFlag(bool image_flag);

private:
    QString m_name;
    QString m_text_massege;
    bool m_image_flag;
};

#endif // MASSEGE_H
