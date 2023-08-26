//
// Created by User on 2023/08/21.
//

#include <iostream>
#include "Cell.h"
#include "TiledModel_ImageGrid.h"


Cell::Cell(int x, int y)
{
    m_position = std::make_pair(x, y);
    m_is_collapsed = false;
    m_collapsed_state = nullptr;
    m_available_states = std::vector<CellState*>();
    m_previous = nullptr;
    m_is_current_cell = false;
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
    if(m_collapsed_state == nullptr)
        std::cout << "collapsed to nothing" << std::endl;

    m_is_collapsed = true;
    emit cell_collapsed(1);
}

void Cell::set_collapsed_state(CellState *state)
{
    remove_state(state);
    m_collapsed_state = state;
    m_is_collapsed = true;
    emit cell_collapsed(1);
}

bool Cell::is_collapsed() const
{
    return m_is_collapsed;
}

const std::pair<int, int> &Cell::get_position()
{
    return m_position;
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
        auto new_name = state->get_name();
        auto new_symmetry = state -> get_symmetry();
        auto new_orientation = state -> get_orientation();
        auto new_state = new CellState(state->index(), new_name, new_symmetry, new_orientation, pixmap_copy);
        new_available_states.emplace_back(new_state);
    }
    clear_available_states();
    m_available_states = new_available_states;
}

unsigned int Cell::get_entropy() const
{
    if(m_is_current_cell)
        return INT_MAX;
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

void Cell::make_current_block(bool is_current)
{
    m_is_current_cell = is_current;
}

void Cell::set_previous_cell(Cell *cell)
{
    m_previous = cell;
}

void Cell::remove_state(CellState *state)
{
    m_available_states.erase(std::remove_if(m_available_states.begin(), m_available_states.end(),
                                            [&](CellState* s) -> bool{
        return state != nullptr && s != nullptr && state -> get_name() == s -> get_name();
    }), m_available_states.end());
}

