//
// Created by User on 2023/08/15.
//

#include "GridTile.h"

int GridTile::s_advance_call_count = 0;

GridTile::GridTile(SudokuBlock *model, int x, int y)
{
    m_tile_model = model;
    m_value = new QLabel(QString::number(model -> get_collapsed_state() -> get_value()));
    QObject::connect(m_tile_model, SIGNAL(block_collapsed(int)), this, SLOT(advance(int)));

    if(!(m_tile_model -> get_is_permanently_collapsed()))
    {
        int state_index = 0;
        for(auto i = 0; i < 3; i++)
        {
            QList<QLabel*> row;
            for(auto j = 0; j < 3; j++)
            {
                auto state_label = new QLabel;

                if(state_index < m_tile_model -> get_available_states().size())
                {
                    const auto& state_model = m_tile_model -> get_available_states().at(state_index);
                    state_label ->setText(QString::number(state_model -> get_value()));
                }
                else
                {
                    state_label ->setText(" ");
                }
                row.emplace_back(state_label);
                state_index++;
            }
            m_available_states.emplace_back(row);
        }
    }

    m_position = std::make_pair(x + POSITION_OFFSET, y + POSITION_OFFSET);
    m_is_permanently_collapsed = model -> get_is_permanently_collapsed();
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
    auto superpositionPen = QPen(QColor(189, 43, 12));
    if(m_value -> text().toInt() != 0)
    {
        m_is_permanently_collapsed ? painter ->setPen(permanentlyCollapsedPen) : painter ->setPen(solvedTextPen);
        painter ->setFont(QFont("Arial", 20, QFont::Bold));
        painter ->drawText(boundingRect(), Qt::AlignCenter,  m_value -> text());
    }
    else
    {
        painter ->setPen(permanentlyCollapsedPen);
        painter ->setFont(QFont("Arial", 12, QFont::DemiBold));
        for(auto row = 0; row < 3; row++)
        {
            for(auto col = 0; col < 3; col++)
            {
                auto x = row * STATES_LABEL_SIZE + POSITION_OFFSET - PEN_WIDTH / 2.0;
                auto y = col * STATES_LABEL_SIZE + POSITION_OFFSET - PEN_WIDTH / 2.0;
                const auto& state_label = m_available_states.at(row).at(col);
                painter ->drawText((int)x, (int)y, STATES_LABEL_SIZE, STATES_LABEL_SIZE, Qt::AlignCenter, state_label -> text());
            }
        }
    }
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

void GridTile::advance(int step) {
    if(!step) return;
    update(this -> boundingRect());
    m_value ->setText(QString::number(m_tile_model -> get_collapsed_state() -> get_value()));
    qDebug() << "Advance call count: " << s_advance_call_count;
    qDebug() << "Advances done: " << std::get<0>(m_tile_model -> get_coordinate()) << " " << std::get<1>(m_tile_model -> get_coordinate());
}

GridTile::~GridTile()
{
    qDebug() << "Destroying a tile: ";
}

