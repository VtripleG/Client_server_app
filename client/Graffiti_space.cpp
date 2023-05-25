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
    m_points_vector.push_back(event->pos());
    qDebug()<<"class mouse event";
    update();
}

void Graffiti_space::drawPoint(QPoint point)
{
    m_image_painter.begin(&m_graffiti);
    m_image_painter.drawPoint(point);
    update();
}

void Graffiti_space::paintEvent(QPaintEvent *event)
{
    qDebug() << "start painting";
    QPainter image_painter(this);
    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(10);
    image_painter.setPen(pen);
    for(auto &point: m_points_vector)
    {
        image_painter.drawPoint(point);
    }
}

