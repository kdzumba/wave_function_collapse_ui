//
// Created by User on 2023/08/16.
//

#include <QGraphicsGridLayout>
#include "BoardContainer.h"

BoardContainer::BoardContainer(QGraphicsItem *parent, int size) : QGraphicsWidget(parent), m_size(size)
{
    m_main_grid_layout = new QGraphicsGridLayout(this);
    m_main_grid_layout->setSpacing(2);

    m_inner_grid_layouts = QList<QGraphicsGridLayout*>();
    m_inner_grid_widgets = QList<QGraphicsWidget*>();

    for(std::size_t index = 0; index < m_size; index++)
    {
        auto inner_grid_widget = new QGraphicsWidget(this);
        auto inner_grid_layout = new QGraphicsGridLayout(inner_grid_widget);

        inner_grid_layout ->setSpacing(0);
        QObject::connect(inner_grid_widget, SIGNAL(geometryChanged()), this, SLOT(childGridGeometryChanged()));
        m_inner_grid_widgets.emplace_back(inner_grid_widget);
        m_inner_grid_layouts.emplace_back(inner_grid_layout);
    }
}

//Adds the 3x3 square blocks into the main sudoku container
void BoardContainer::add(QGraphicsLayout *item, int row, int col, Qt::Alignment alignment)
{
    m_main_grid_layout ->addItem(item, row, col, alignment);
}

QList<QGraphicsWidget*> BoardContainer::getInnerGridWidgets()
{
    return m_inner_grid_widgets;
}

QList<QGraphicsGridLayout*> BoardContainer::getInnerGridLayouts()
{
    return m_inner_grid_layouts;
}

std::size_t BoardContainer::getSize() const { return m_size; }

void BoardContainer::childGridGeometryChanged()
{
    this -> layout() -> invalidate();
    this -> layout()->activate();
}
