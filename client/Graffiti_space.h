#ifndef GRAFFITI_SPACE_H
#define GRAFFITI_SPACE_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QImage>
#include <QMouseEvent>
#include <QPainter>

class Graffiti_space : public QGraphicsView
{
    Q_OBJECT
public:
    void mousePressEvent(QMouseEvent * event);
    Graffiti_space(QWidget *parent = nullptr);
private:
    QImage * m_graffiti = new QImage(660, 460, QImage::Format_RGB32);
    QPainter * m_image_painter = new QPainter;
    QPainter * m_scene_painter = new QPainter;
};

#endif // GRAFFITI_SPACE_H
