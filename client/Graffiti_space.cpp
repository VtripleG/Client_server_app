#include "Graffiti_space.h"
#include <QBuffer>
#include <QByteArray>

void Graffiti_space::mousePressEvent(QMouseEvent *event)
{
    m_image_painter->begin(m_graffiti);
    m_image_painter->drawPoint(event->pos().x(), event->pos().y());
    QByteArray byte_array;
    QBuffer buff(&byte_array);
    m_graffiti->save(&buff, "png");
    m_scene_painter->begin(this);
    m_scene_painter->drawImage(QPoint(0, 0), QImage::fromData(byte_array, "png"));
}

Graffiti_space::Graffiti_space(QWidget *parent)
{
}


