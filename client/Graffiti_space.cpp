#include "Graffiti_space.h"
#include <QBuffer>
#include <QByteArray>
#include <QGraphicsScene>

Graffiti_space::Graffiti_space(QWidget *parent):
    QWidget(parent)
{
    setMouseTracking(true);
}

void Graffiti_space::mousePressEvent(QMouseEvent *event)
{
    m_line_flag = true;
    str_point = event->pos();
//    m_points_vector.push_back(event->pos());
    drawLine(event->pos());
    qDebug()<<"class mouse event";
    update();
}

void Graffiti_space::mouseMoveEvent(QMouseEvent *event)
{
    if(m_line_flag)
    {
        drawLine(event->pos());
        str_point = event->pos();
//        m_points_vector.push_back(event->pos());
        update();
    }

}

void Graffiti_space::mouseReleaseEvent(QMouseEvent *event)
{
    m_line_flag = false;
}

void Graffiti_space::drawLine(QPoint point)
{
    QPainter image_painter(&m_graffiti);
    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(6);
    image_painter.setPen(pen);
    image_painter.drawLine(str_point, point);
//    image_painter.drawPoint(point);
//    update();
}

void Graffiti_space::paintEvent(QPaintEvent *event)
{
    qDebug() << "start painting";
    QPainter painter(this);
//    QPen pen;
//    pen.setColor(Qt::red);
//    pen.setWidth(10);
//    painter.setPen(pen);
//    for(auto &point: m_points_vector)
//    {
//        painter.drawPoint(point);
//    }
    painter.drawImage(0, 0, m_graffiti);
}

