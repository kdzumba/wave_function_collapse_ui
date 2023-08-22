//
// Created by User on 2023/08/21.
//

#ifndef WAVE_FUNCTION_COLLAPSE_UI_CELL_H
#define WAVE_FUNCTION_COLLAPSE_UI_CELL_H

#include <vector>
#include <tuple>

class Cell
{
public:
    Cell(int x, int y);
    void collapse(int state);
    bool is_collapsed();
    int get_collapsed_image_index();
    const std::tuple<int,int>& get_position();
private:
    bool m_is_collapsed;
    int m_collapsed_index{};
    std::vector<int> m_available_states;
    std::tuple<int, int> m_position;
};

#endif //WAVE_FUNCTION_COLLAPSE_UI_CELL_H
