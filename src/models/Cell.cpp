//
// Created by User on 2023/08/21.
//

#include "Cell.h"
#include "TiledModel_ImageGrid.h"


Cell::Cell(int x, int y)
{
    m_position = std::make_tuple(x, y);
    m_is_collapsed = false;
    m_collapsed_state = nullptr;

    auto available_states_count = TiledModel_ImageGrid::available_states_count();
    m_available_states = std::vector<CellState*>();
}

void Cell::collapse(int index)
{
    auto state_found = [=](CellState* state) -> bool{return state -> index() == index;};
    m_collapsed_state = *(std::find_if(m_available_states.begin(), m_available_states.end(), state_found));
    m_is_collapsed = true;
}

bool Cell::is_collapsed()
{
    return m_is_collapsed;
}

int Cell::collapsed_state_index()
{
    return m_collapsed_state->index();
}

const std::tuple<int, int> &Cell::position()
{
    return m_position;
}

void Cell::remove_state(int index)
{
    m_available_states.erase(std::remove_if(m_available_states.begin(), m_available_states.end(),
                                            [=](CellState* state) -> bool {
        return state -> index() == index;
    }), m_available_states.end());
}

CellState *Cell::state()
{
    return m_collapsed_state;
}

void Cell::set_available_states(std::vector<CellState *> states)
{
    m_available_states = states;
}
