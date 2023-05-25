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
    void setColor(QColor color);
    QImage getImage();

private:
    void drawLine(QPoint point);

    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void paintEvent(QPaintEvent *event);

    QVector <QPoint> m_points_vector;
    bool m_line_flag = false;

    QImage m_graffiti = QImage(660, 460, QImage::Format_RGBA64);

    QPoint m_str_point;
    QColor m_color = QColor(Qt::red);
};

#endif // GRAFFITI_SPACE_H
