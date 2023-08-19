//
// Created by User on 2023/08/17.
//

#include <QHBoxLayout>
#include "ButtonGroup.h"

ButtonGroup::ButtonGroup(QWidget* parent, QBoxLayout::Direction direction) :
QWidget(parent, Qt::FramelessWindowHint | Qt::WindowSystemMenuHint)
{
    m_layout = new QBoxLayout(direction, parent);
    m_layout ->setContentsMargins(0, 0, 0, 0);
}

void ButtonGroup::add(QAbstractButton *button)
{
    m_layout -> addWidget(button);
}

