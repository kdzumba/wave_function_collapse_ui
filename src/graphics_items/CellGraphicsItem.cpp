//
// Created by User on 2023/08/23.
//

#include "CellGraphicsItem.h"
#include <iostream>
#include <QPen>

CellGraphicsItem::CellGraphicsItem(Cell *model, int x, int y)
{
    m_cell_model = model;
    QObject::connect(m_cell_model, SIGNAL(cell_collapsed(int)), this, SLOT(advance(int)));
}

void CellGraphicsItem::setGeometry(const QRectF &geometry)
{
    prepareGeometryChange();
    QGraphicsLayoutItem::setGeometry(geometry);
    setPos(geometry.topLeft());
}

QSizeF CellGraphicsItem::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    switch(which)
    {
        case Qt::MinimumSize:
        case Qt::PreferredSize:
        case Qt::MaximumSize:
            return {32, 32};
        default:
            break;
    }
    return {};
}

void CellGraphicsItem::advance(int step)
{
    if(!step) return;
    auto pixmap = m_cell_model->get_state() -> pixmap();
    this ->setPixmap(*pixmap);
    this ->setTransformOriginPoint(this->boundingRect().center());
    this ->setRotation(m_cell_model -> get_state() -> get_orientation() * -90);
}

QRectF CellGraphicsItem::boundingRect() const
{
    return {0, 0, IMAGE_WIDTH, IMAGE_HEIGHT};

}
