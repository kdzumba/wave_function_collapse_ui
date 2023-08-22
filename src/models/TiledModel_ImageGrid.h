//
// Created by User on 2023/08/22.
//

#ifndef WAVE_FUNCTION_COLLAPSE_UI_TILEDMODEL_IMAGEGRID_H
#define WAVE_FUNCTION_COLLAPSE_UI_TILEDMODEL_IMAGEGRID_H

#include <vector>
#include <tuple>
#include <QBitmap>
#include <QObject>
#include "Cell.h"

class TiledModel_ImageGrid : public QObject
{
public:
    TiledModel_ImageGrid(int width, int height, const std::string& image_dir);
    static int available_states_count();
    void load_cell_states(const std::string& img_directory);
    const std::tuple<int, int>& dimensions();
    std::vector<std::vector<Cell*>> grid();
    std::vector<QPixmap*> states();
private:
    std::vector<std::vector<Cell*>> m_grid;
    static int s_available_states_count;
    std::tuple<int, int> m_dimensions;
    std::vector<QPixmap*> m_states;
};
#endif //WAVE_FUNCTION_COLLAPSE_UI_TILEDMODEL_IMAGEGRID_H
