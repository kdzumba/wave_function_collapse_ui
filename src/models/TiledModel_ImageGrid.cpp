//
// Created by User on 2023/08/22.
//

#include <filesystem>
#include "TiledModel_ImageGrid.h"

int TiledModel_ImageGrid::s_available_states_count = 0;

TiledModel_ImageGrid::TiledModel_ImageGrid(int width, int height, const std::string& image_dir)
{
    m_dimensions = std::make_tuple(width, height);
    load_cell_states(image_dir);

    for(auto row = 0; row < width; row++)
    {
        auto row_ = std::vector<Cell*>();
        for(auto col = 0; col < height; col++)
        {
            auto cell = new Cell(row, col);
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

    for(const auto& file : std::filesystem::directory_iterator(img_directory))
    {
        const auto& filename = file.path();
        auto pixmap = new QPixmap(QString::fromStdString(filename.string()));
        m_states.emplace_back(pixmap);
        s_available_states_count++;
    }
}

std::vector<std::vector<Cell *>> TiledModel_ImageGrid::grid()
{
    return m_grid;
}

const std::tuple<int, int> &TiledModel_ImageGrid::dimensions()
{
    return m_dimensions;
}

std::vector<QPixmap *> TiledModel_ImageGrid::states()
{
    return m_states;
}
