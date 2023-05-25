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
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void drawLine(QPoint point);
    void paintEvent(QPaintEvent *event);

    QImage m_graffiti = QImage(660, 460, QImage::Format_RGBA64);



private:
    QVector <QPoint> m_points_vector;
    bool m_line_flag = false;

    QPoint str_point;
    QPoint end_point;
};

#endif // GRAFFITI_SPACE_H
