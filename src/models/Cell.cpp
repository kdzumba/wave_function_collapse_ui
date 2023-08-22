//
// Created by User on 2023/08/21.
//

#include "Cell.h"
#include "TiledModel_ImageGrid.h"


Cell::Cell(int x, int y)
{
    m_position = std::make_tuple(x, y);
    m_is_collapsed = false;
    m_available_states = std::vector<int>(TiledModel_ImageGrid::available_states_count());
    std::iota(m_available_states.begin(), m_available_states.end(), 1);
}

void Cell::collapse(int state)
{
    m_collapsed_index = state;
    m_is_collapsed = true;
}

bool Cell::is_collapsed()
{
    return m_is_collapsed;
}

int Cell::get_collapsed_image_index()
{
    return m_collapsed_index;
}

const std::tuple<int, int> &Cell::get_position()
{
    return m_position;
}
