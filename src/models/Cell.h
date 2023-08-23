//
// Created by User on 2023/08/21.
//

#ifndef WAVE_FUNCTION_COLLAPSE_UI_CELL_H
#define WAVE_FUNCTION_COLLAPSE_UI_CELL_H

#include <vector>
#include <tuple>
#include "CellState.h"

class Cell
{
public:
    Cell(int x, int y);
    void collapse(int index);
    bool is_collapsed();
    void remove_state(int index);
    int collapsed_state_index();
    CellState* state();
    const std::tuple<int,int>& position();
    void set_available_states(std::vector<CellState*> states);
private:
    CellState* m_collapsed_state;
    std::vector<CellState*> m_available_states;
    bool m_is_collapsed;
    std::tuple<int, int> m_position;
};

#endif //WAVE_FUNCTION_COLLAPSE_UI_CELL_H
