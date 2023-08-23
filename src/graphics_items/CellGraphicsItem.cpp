//
// Created by User on 2023/08/23.
//

#include "CellGraphicsItem.h"

CellGraphicsItem::CellGraphicsItem(Cell *model, int x, int y)
{
    m_cell_model = model;
    auto pixmap = m_cell_model->get_state() -> pixmap();
    this ->setPixmap(*pixmap);
    m_position = std::make_pair(x * pixmap -> width(), y * pixmap -> height());
    this ->setPos(m_position.first, m_position.second);
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
            return {14, 14};
        default:
            break;
    }
    return {};
}
