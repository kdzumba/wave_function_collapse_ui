//
// Created by User on 2023/08/22.
//

#include <filesystem>
#include "TiledModel_ImageGrid.h"
#include "../utils.h"

int TiledModel_ImageGrid::s_available_states_count = 0;

TiledModel_ImageGrid::TiledModel_ImageGrid(int width, int height, const std::string& image_dir)
{
    m_dimensions = std::make_pair(width, height);
    load_cell_states(image_dir);

    for(auto row = 0; row < width; row++)
    {
        auto row_ = std::vector<Cell*>();
        for(auto col = 0; col < height; col++)
        {
            auto cell = new Cell(row, col);
            cell ->set_available_states(m_all_states);

            auto state_index = Utils::generate_random_int(1, 13);
            cell ->collapse(state_index);
            cell ->state() ->set_pixmap(index_state_mapping().at(state_index));
            row_.emplace_back(cell);
        }
        m_grid.emplace_back(row_);
    }
}

int TiledModel_ImageGrid::available_states_count()
{
    return s_available_states_count;
}

void TiledModel_ImageGrid::load_cell_states(const std::string &img_directory)
{
    s_available_states_count = 0;
    int state_index = 1;
    m_all_states = std::vector<CellState*>();

    for(const auto& file : std::filesystem::directory_iterator(img_directory))
    {
        const auto& filename = file.path();
        auto pixmap = new QPixmap(QString::fromStdString(filename.string()));
        m_index_state_map.insert(std::make_pair(state_index, pixmap));
        m_all_states.emplace_back(new CellState(state_index, pixmap));
        s_available_states_count++;
        state_index++;
    }
}

std::vector<std::vector<Cell *>> TiledModel_ImageGrid::grid()
{
    return m_grid;
}

const std::pair<int, int> &TiledModel_ImageGrid::dimensions()
{
    return m_dimensions;
}

std::map<int, QPixmap *> TiledModel_ImageGrid::index_state_mapping()
{
    return m_index_state_map;
}
