//
// Created by User on 2023/09/09.
//

#include "SliderGroup.h"

SliderGroup::SliderGroup(QWidget *parent, QBoxLayout::Direction direction):
        QWidget(parent, Qt::FramelessWindowHint | Qt::WindowSystemMenuHint)
{
    m_layout = new QBoxLayout(direction, parent);
    m_layout ->setContentsMargins(0, 0, 0, 0);
}

void SliderGroup::add(QAbstractSlider *slider)
{
    m_layout ->addWidget(slider);
}

void SliderGroup::addLayout(QLayout *layout)
{
    m_layout->addLayout(layout);
}
