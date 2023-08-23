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
            row_.emplace_back(cell);
        }
        m_grid.emplace_back(row_);
    }

    init_generation();
    generate();
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

void TiledModel_ImageGrid::init_generation()
{
    collapse(get_initial_cell());
}

Cell *TiledModel_ImageGrid::get_initial_cell()
{
    std::vector<std::pair<int,int>> positions_to_consider;
    for(const auto& row : m_grid)
    {
        for(const auto& cell : row)
        {
            positions_to_consider.emplace_back(cell->get_position());
        }
    }
    auto rand_index = Utils::generate_random_int(0, (int) positions_to_consider.size());
    auto initial_coordinate = positions_to_consider.at(rand_index);
    return m_grid.at(initial_coordinate.first).at(initial_coordinate.second);
}

void TiledModel_ImageGrid::collapse(Cell *cell)
{
    auto rand_index = Utils::generate_random_int(1, (int) cell -> get_available_states().size());
    cell ->collapse(rand_index);
}

bool TiledModel_ImageGrid::is_fully_generated() {
    for(const auto& row : m_grid)
    {
        for(const auto& cell : row)
        {
            //Collapsed index less than 0 means the cell  hasn't been collapsed
            if(cell->get_state() == nullptr)
                return false;
        }
    }
    return true;
}

void TiledModel_ImageGrid::generate()
{
    auto next_cell = least_entropy_cell();
    collapse(next_cell);
    if(!(is_fully_generated()))
        generate();
    else
        return;
}

Cell *TiledModel_ImageGrid::least_entropy_cell()
{
    std::map<unsigned int, std::vector<Cell*>> entropy_to_cell_map;
    for(const auto& row : m_grid)
    {
        for(const auto& cell : row)
        {
            unsigned int key = cell -> get_entropy();
            if(entropy_to_cell_map.find(key) != entropy_to_cell_map.end() && key != 0)
            {
                entropy_to_cell_map.at(key).emplace_back(cell);
            }
            else
            {
                auto value = std::vector<Cell*>{cell};
                entropy_to_cell_map.insert({key, value});
            }
        }
    }
    auto min_entropy = entropy_to_cell_map.begin() -> second;
    auto rand_index = Utils::generate_random_int(0, (int)min_entropy.size() - 1);
    auto least_entropy_cell = min_entropy.at(rand_index);
    return least_entropy_cell;
}
