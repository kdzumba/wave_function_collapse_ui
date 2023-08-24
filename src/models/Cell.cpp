//
// Created by User on 2023/08/21.
//

#include "Cell.h"
#include "TiledModel_ImageGrid.h"


Cell::Cell(int x, int y)
{
    m_position = std::make_pair(x, y);
    m_is_collapsed = false;
    m_collapsed_state = nullptr;
    m_available_states = std::vector<CellState*>();
}

void Cell::collapse(int index)
{
    //An index of 0 should reset the collapse of the cell
    if(index == 0)
    {
        m_collapsed_state = nullptr;
        m_is_collapsed = false;
        return;
    }

    auto state_found = [=](CellState* state) -> bool{return state -> index() == index;};
    m_collapsed_state = *(std::find_if(m_available_states.begin(), m_available_states.end(), state_found));
    m_is_collapsed = true;
    emit cell_collapsed(1);
}

bool Cell::is_collapsed()
{
    return m_is_collapsed;
}

int Cell::get_collapsed_state_index()
{
    return m_collapsed_state->index();
}

const std::pair<int, int> &Cell::get_position()
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

CellState *Cell::get_state()
{
    return m_collapsed_state;
}

void Cell::set_available_states(const std::vector<CellState *>& states)
{
    auto new_available_states = std::vector<CellState*>();

    for(const auto& state : states)
    {
        auto pixmap_copy = new QPixmap(*(state->pixmap()));
        auto new_state = new CellState(state->index(), pixmap_copy);
        new_available_states.emplace_back(new_state);
    }
    clear_available_states();
    m_available_states = new_available_states;
}

unsigned int Cell::get_entropy() const
{
    return m_available_states.size();
}

std::vector<CellState *> Cell::get_available_states()
{
    return m_available_states;
}

void Cell::clear_available_states()
{
    for(auto state : m_available_states)
    {
        delete state;
    }
    m_available_states.clear();
}
