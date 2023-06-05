#include "Graffiti_space.h"

Graffiti_space::Graffiti_space(QWidget *parent):
    QWidget(parent)
{
    setMouseTracking(true);
    clearImage();
}

void Graffiti_space::setColor(QColor color)
{
    m_color = color;
}

QString Graffiti_space::getImage()
{
    m_graffiti_chanched = false;
    QByteArray byte_ar;
    QBuffer cod_buff (&byte_ar);
    m_graffiti.save(&cod_buff, "png");
    QString string = byte_ar.toBase64();
    return string;
}

void Graffiti_space::clearImage()
{
    m_graffiti.fill(qRgb(0, 0, 0));
}

void Graffiti_space::setImage(QImage back_ground)
{
    clearImage();
    if(back_ground.width() > back_ground.height())
    {
        back_ground = back_ground.scaled(QSize(660, (back_ground.height()/(back_ground.width()/660))), Qt::KeepAspectRatio);
    }
    else
    {
        back_ground = back_ground.scaled(QSize((back_ground.width()/(back_ground.height()/460)), 460), Qt::KeepAspectRatio);
    }
    m_graffiti = back_ground;
    repaint();
}

void Graffiti_space::setBackground(QImage back_ground)
{
    clearImage();
    if(back_ground.width() > back_ground.height())
    {
        back_ground = back_ground.scaled(QSize(660, (back_ground.height()/(back_ground.width()/660))), Qt::KeepAspectRatio);
    }
    else
    {
        back_ground = back_ground.scaled(QSize((back_ground.width()/(back_ground.height()/460)), 460), Qt::KeepAspectRatio);
    }
    m_graffiti = back_ground;
    update();
}

bool Graffiti_space::graffitiChanched()
{
    return m_graffiti_chanched;
}

void Graffiti_space::mousePressEvent(QMouseEvent *event)
{
    m_line_flag = true;
    m_str_point = event->pos();
    drawLine(event->pos());
    update();
}

void Graffiti_space::mouseMoveEvent(QMouseEvent *event)
{
    if(m_line_flag)
    {
        drawLine(event->pos());
        m_str_point = event->pos();
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
    pen.setColor(m_color);
    pen.setWidth(6);
    image_painter.setPen(pen);
    image_painter.drawLine(m_str_point, point);
    update();
    m_graffiti_chanched = true;
    emit(clicked());
}

void Graffiti_space::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawImage(0, 0, m_graffiti);
}

