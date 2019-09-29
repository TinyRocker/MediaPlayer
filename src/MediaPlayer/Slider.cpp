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
    if (e->button() == Qt::LeftButton)
    {
        setValue((double)e->pos().x() / width() * maximum());
        e->accept();   
        QSlider::sliderPressed();
    }
}

void Slider::mouseReleaseEvent(QMouseEvent * e)
{
    e->accept();
    QSlider::sliderReleased();
}

void Slider::mouseMoveEvent(QMouseEvent * e)
{
    if (e->buttons() & Qt::LeftButton)
    {
        setValue((double)e->pos().x() / width() * maximum());
        e->accept();
        QSlider::sliderMoved(e->pos().x());
    }
}
