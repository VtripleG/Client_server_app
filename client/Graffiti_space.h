#ifndef GRAFFITI_SPACE_H
#define GRAFFITI_SPACE_H

#include <QGraphicsView>
#include <QWidget>
#include <QMouseEvent>
#include <QImage>
#include <QMouseEvent>
#include <QPainter>

class Graffiti_space : public QWidget
{
    Q_OBJECT
public:
    Graffiti_space(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent * event);
    void drawPoint(QPoint point);
//    void paintEvent(QPaintEvent *event);

    QImage m_graffiti = QImage(660, 460, QImage::Format_RGB32);
    QPainter m_image_painter;
    QPainter m_widget_painter;



private:
//    QVector <QPoint> m_points_vector;
};

#endif // GRAFFITI_SPACE_H
