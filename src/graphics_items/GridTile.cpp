//
// Created by User on 2023/08/15.
//

#include "GridTile.h"

GridTile::GridTile(const QString& value, int x, int y, bool isPermanantelyCollapsed)
{
    m_value = new QLabel(value);
    m_position = std::make_pair(x + POSITION_OFFSET, y + POSITION_OFFSET);
    m_is_permanently_collapsed = isPermanantelyCollapsed;
    this ->setPos(m_position.first, m_position.second);
}

QRectF GridTile::boundingRect() const
{
    return {POSITION_OFFSET - PEN_WIDTH / 2.0, POSITION_OFFSET - PEN_WIDTH / 2.0, TILE_SIZE, TILE_SIZE};
}

void GridTile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter ->setBrush(Qt::white);
    auto boxPen = QPen(Qt::gray);
    boxPen.setWidth(PEN_WIDTH);
    painter ->setPen(boxPen);
    painter ->drawRoundedRect(boundingRect(), 0, 0);
    auto permanentlyCollapsedPen = QPen(QColor(76, 79, 78 ));
    auto solvedTextPen = QPen(QColor(12, 215, 151 ));
    m_is_permanently_collapsed ? painter ->setPen(permanentlyCollapsedPen) : painter ->setPen(solvedTextPen);
    painter ->setFont(QFont("Arial", 20, QFont::Bold));

    if(m_value -> text().toInt() != 0)
        painter ->drawText(boundingRect(), Qt::AlignCenter,  m_value -> text());
}

void GridTile::setGeometry(const QRectF &geometry)
{
    prepareGeometryChange();
    QGraphicsLayoutItem::setGeometry(geometry);
    setPos(geometry.topLeft());
}

QSizeF GridTile::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    switch(which)
    {
        case Qt::MinimumSize:
        case Qt::PreferredSize:
        case Qt::MaximumSize:
            return {TILE_SIZE, TILE_SIZE};
        default:
            break;
    }
    return {};
}
