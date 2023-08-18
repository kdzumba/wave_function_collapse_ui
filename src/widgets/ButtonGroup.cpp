//
// Created by User on 2023/08/17.
//

#include <QHBoxLayout>
#include "ButtonGroup.h"

ButtonGroup::ButtonGroup(QWidget* parent, QBoxLayout::Direction direction) : QWidget(parent)
{
    m_layout = new QBoxLayout(direction, parent);
}

void ButtonGroup::add(QAbstractButton *button)
{
    m_layout -> addWidget(button);
}
