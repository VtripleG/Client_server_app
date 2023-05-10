#include "Massege.h"


Massege::Massege(QString name, QString massege_string, bool image_flag)
{
    this->m_image_flag = image_flag;
    this->m_name = name;
    this->m_text_massege = massege_string;
}

QString Massege::getName()
{
    return m_name;
}

QString Massege::getText()
{
    return m_text_massege;
}

bool Massege::getImageFlag()
{
    return m_image_flag;
}

void Massege::setName(QString name)
{
    m_name = name;
}

void Massege::setText(QString text_massege)
{
    m_text_massege = text_massege;
}

void Massege::setImageFlag(bool image_flag)
{
    m_image_flag = image_flag;
}


