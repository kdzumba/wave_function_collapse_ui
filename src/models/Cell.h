//
// Created by User on 2023/08/21.
//

#ifndef WAVE_FUNCTION_COLLAPSE_UI_CELL_H
#define WAVE_FUNCTION_COLLAPSE_UI_CELL_H

#include <vector>
#include "CellState.h"
#include <QObject>

class Cell : public QObject
{
    Q_OBJECT
public:
    Cell(int x, int y);
    void reset();
    void set_collapsed_state(CellState* state);
    void remove_state(CellState* state);
    bool is_collapsed() const;
    void clear_available_states();
    CellState* get_state();
    std::vector<CellState*> get_available_states();
    unsigned int get_entropy() const;
    const std::pair<int,int>& get_position();
    void set_available_states(const std::vector<CellState*>& states);
    void make_current_block(bool is_current);
    void set_previous_cell(Cell* cell);
signals:
    void cell_collapsed(int phase);
private:
    CellState* m_collapsed_state;
    std::vector<CellState*> m_available_states;
    bool m_is_collapsed;
    std::pair<int, int> m_position;
    bool m_is_current_cell;
    Cell* m_previous;
};

#endif //WAVE_FUNCTION_COLLAPSE_UI_CELL_H
