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
    void collapse(int index);
    bool is_collapsed();
    void clear_available_states();
    void remove_state(int index);
    int get_collapsed_state_index();
    CellState* get_state();
    std::vector<CellState*> get_available_states();
    unsigned int get_entropy() const;
    const std::pair<int,int>& get_position();
    void set_available_states(const std::vector<CellState*>& states);
signals:
    void cell_collapsed(int phase);
private:
    CellState* m_collapsed_state;
    std::vector<CellState*> m_available_states;
    bool m_is_collapsed;
    std::pair<int, int> m_position;
};

#endif //WAVE_FUNCTION_COLLAPSE_UI_CELL_H
