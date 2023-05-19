#ifndef GRAFFITI_SPACE_H
#define GRAFFITI_SPACE_H

#include <QGraphicsView>
#include <QMouseEvent>

class Graffiti_space : public QGraphicsView
{
    Q_OBJECT
public:
    void mousePressEvent(QMouseEvent * event);
    Graffiti_space(QWidget *parent = nullptr);
};

#endif // GRAFFITI_SPACE_H
