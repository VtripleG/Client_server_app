#include "Graffiti_space.h"
#include <QBuffer>
#include <QByteArray>
#include <QGraphicsScene>

Graffiti_space::Graffiti_space(QWidget *parent):
    QWidget(parent)
{
}

void Graffiti_space::mousePressEvent(QMouseEvent *event)
{
//    m_points_vector.push_back(event->pos());
    qDebug()<<"class mouse event";

}

void Graffiti_space::drawPoint(QPoint point)
{
    m_image_painter.begin(&m_graffiti);
    m_image_painter.drawPoint(point);
    update();
}

//void Graffiti_space::paintEvent(QPaintEvent *event)
//{
//    qDebug() << "start painting";
//    m_image_painter.begin(this);
//    QPen pen;
//    pen.setColor(QColor(1, 0, 0));
//    pen.setWidth(10);
//    m_image_painter.setPen(pen);
//    for(auto &point: m_points_vector)
//    {
//        m_image_painter.drawPoint(point);
//    }
//}

