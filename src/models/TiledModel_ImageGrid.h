//
// Created by User on 2023/08/22.
//

#ifndef WAVE_FUNCTION_COLLAPSE_UI_TILEDMODEL_IMAGEGRID_H
#define WAVE_FUNCTION_COLLAPSE_UI_TILEDMODEL_IMAGEGRID_H

#include <map>
#include <QBitmap>
#include <QObject>
#include "Cell.h"

class TiledModel_ImageGrid : public QObject
{
public:
    TiledModel_ImageGrid(int width, int height, const std::string& image_dir);
    static int available_states_count();
    void load_cell_states(const std::string& img_directory);
    const std::pair<int, int>& dimensions();
    std::vector<std::vector<Cell*>> grid();
    std::map<int, QPixmap*> index_state_mapping();
    void init_generation();
    Cell* get_initial_cell();
    static void collapse(Cell* cell);
    bool is_fully_generated();
    void generate();
    void reset();
    Cell* least_entropy_cell();

private:
    std::vector<std::vector<Cell*>> m_grid;
    static int s_available_states_count;
    std::pair<int, int> m_dimensions;
    std::map<int, QPixmap*> m_index_state_map;
    std::vector<CellState*> m_all_states;
};
#endif //WAVE_FUNCTION_COLLAPSE_UI_TILEDMODEL_IMAGEGRID_H
