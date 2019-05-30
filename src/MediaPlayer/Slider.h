#pragma once

#include <QSlider>

class Slider : public QSlider
{
    Q_OBJECT

public:
    Slider(QWidget *parent);
    ~Slider();
protected:
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *ev);
    virtual void mouseMoveEvent(QMouseEvent *ev);
};
