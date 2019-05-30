#include "Slider.h"
#include <QMouseEvent>

Slider::Slider(QWidget *parent)
    : QSlider(parent)
{
}

Slider::~Slider()
{
}

void Slider::mousePressEvent(QMouseEvent * e)
{
    setValue((double)e->pos().x() / width() * maximum());
    
    e->accept();
    QSlider::sliderPressed();
}

void Slider::mouseReleaseEvent(QMouseEvent * e)
{
    e->accept();
    QSlider::sliderReleased();
}

void Slider::mouseMoveEvent(QMouseEvent * e)
{
    setValue((double)e->pos().x() / width() * maximum());
    e->accept();
    QSlider::sliderMoved(e->pos().x());
}
