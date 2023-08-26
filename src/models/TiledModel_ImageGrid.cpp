//
// Created by User on 2023/08/22.
//

#include <filesystem>
#include <QDomDocument>
#include <QFile>
#include <iostream>
#include "TiledModel_ImageGrid.h"
#include "../utils.h"

TiledModel_ImageGrid::TiledModel_ImageGrid(int width, int height, const std::string& image_dir)
{
    m_dimensions = std::make_pair(width, height);
    m_tile_set_doc = new QDomDocument;
    m_initial_cell = nullptr;
    m_current_collapsed = nullptr;

    //Load the tileset and it's defined constraints
    load_tile_set_specification("tilesets/Circuit.xml", image_dir);

    for(auto row = 0; row < width; row++)
    {
        auto row_ = std::vector<Cell*>();
        for(auto col = 0; col < height; col++)
        {
            auto cell = new Cell(row, col);
            //Initially, all cells "are" every possible state at the same time
            cell ->set_available_states(m_all_states);
            row_.emplace_back(cell);
        }
        m_grid.emplace_back(row_);
    }
}

void TiledModel_ImageGrid::load_state_images(const std::string &img_directory)
{
    for(const auto& file : std::filesystem::directory_iterator(img_directory))
    {
        const auto& file_path = file.path();
        //Image name with extension
        auto filename = file_path.filename().string();
        auto pixmap = new QPixmap(QString::fromStdString(img_directory + filename));

        //Remove extension from the filename
        auto state_image_name = filename.substr(0, filename.find_last_of('.'));
        //More than 1 CellState can be created for a pixmap, it just needs to be rotated/reflected
        m_name_image_map.insert(std::make_pair(state_image_name, pixmap));
    }

    for(auto state : m_all_states)
    {
        auto image_map_key = (state -> get_name()).substr(0, state -> get_name().find_last_of(' '));
        auto pixmap = m_name_image_map.at(image_map_key);
        state ->set_pixmap(pixmap);
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

void TiledModel_ImageGrid::collapse(Cell *next_cell)
{
    auto rand_index = Utils::generate_random_int(0, (int) next_cell -> get_available_states().size() - 1);
    auto next_state = next_cell -> get_available_states().at(rand_index);

    next_cell -> set_collapsed_state(next_state);

    next_cell ->make_current_block(true);
    if(m_current_collapsed != nullptr)
        m_current_collapsed ->make_current_block(false);

    if(m_initial_cell != nullptr && next_cell != m_current_collapsed)
    {
        next_cell->set_previous_cell(m_current_collapsed);
    }
    else
    {
        m_initial_cell = next_cell;
    }

    auto x = next_cell -> get_position().first;
    auto y = next_cell -> get_position().second;

    propagate_collapse_info(x, y, next_state);
    m_current_collapsed = next_cell;
}

bool TiledModel_ImageGrid::is_fully_generated() {
    for(const auto& row : m_grid)
    {
        for(const auto& cell : row)
        {
            //Collapsed index less than 0 means the cell  hasn't been collapsed
            if(cell-> get_entropy() > 0)
                return false;
        }
    }
    return true;
}

void TiledModel_ImageGrid::generate()
{
    auto next_cell = least_entropy_cell();
    if(next_cell -> get_entropy() == 0)
        std::cout << "Situation needs backtracking" << std::endl;

    collapse(next_cell);

    if(!(is_fully_generated()))
        generate();
}

Cell *TiledModel_ImageGrid::least_entropy_cell()
{
    std::map<unsigned int, std::vector<Cell*>> entropy_to_cell_map;
    auto possible_cells = std::vector<Cell*>();

    for(const auto& row : m_grid)
    {
        for(const auto& cell : row)
        {
            auto key = cell -> get_entropy();
            if(entropy_to_cell_map.find(key) != entropy_to_cell_map.end())
            {
                entropy_to_cell_map.at(key).emplace_back(cell);
            }
            else if(key != 0)
            {
                auto value = std::vector<Cell*>{cell};
                entropy_to_cell_map.insert({key, value});
            }
        }
    }
    auto min_entropy=  entropy_to_cell_map.begin() -> second;
    auto rand_index = Utils::generate_random_int(0, (int) min_entropy.size() - 1);
    return min_entropy.at(rand_index);
}

void TiledModel_ImageGrid::reset()
{
    std::cout << "RESETTING" << std::endl;
    for(const auto& row : m_grid)
    {
        for(auto cell : row)
        {
            cell ->set_available_states(m_all_states);
            cell -> collapse(0);
            cell ->make_current_block(false);
        }
    }
}

void TiledModel_ImageGrid::load_tile_set_specification(const std::string &spec_file, const std::string& images_path)
{
    auto file = QFile(QString::fromStdString(spec_file));
    if(!file.open(QIODevice::ReadOnly))
        return;

    if(!m_tile_set_doc ->setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    //set element as read from the xml file
    auto set_element = m_tile_set_doc->firstChild();

    //If this node has any child nodes (tiles and neighbours)
    if(!set_element.isNull() && set_element.hasChildNodes())
    {
        auto tiles_element = set_element.firstChild();
        int tile_index = 1;
        auto tile = tiles_element.firstChildElement("tile");

        while(!tile.isNull())
        {
            auto name = tile.attribute("name").toStdString();
            auto symmetry = tile.attribute("symmetry").toStdString();
            auto tile_cardinality = CellState::SYMMETRY_CARDINALITY_MAP.at(symmetry);

            for(int i = 0; i < tile_cardinality; i++)
            {
                auto state_name = i == 0 ? name  : name + " " + std::to_string(i);
                m_all_states.emplace_back(new CellState(tile_index, state_name, symmetry, i));
                tile_index++;
            }
            tile = tile.nextSiblingElement("tile");
        }

        auto neighbours_element = tiles_element.nextSibling();
        auto neighbour = neighbours_element.firstChildElement("neighbor");
        while(!neighbour.isNull())
        {
            auto left = neighbour.attribute("left").toStdString();
            auto right = neighbour.attribute("right").toStdString();
            auto* tiled_model_neighbour = new TiledModelRule(left, right);
            m_rules.emplace_back(tiled_model_neighbour);
            neighbour = neighbour.nextSiblingElement("neighbor");
        }
    }
    load_state_images(images_path);
}

std::map<std::string, QPixmap *> TiledModel_ImageGrid::get_name_image_mapping()
{
    return m_name_image_map;
}

void TiledModel_ImageGrid::propagate_collapse_info(int x, int y, CellState *state)
{
    //Only the left and right cells to currently collapsed need to update their available_states
    Cell* left_cell = nullptr;
    Cell* right_cell = nullptr;

    //0th column doesn't have a left cell neighbour
    if(y > 0)
        left_cell = m_grid.at(x).at(y - 1);

    //Last cell in a row doesn't have a right cell neighbour
    if(y < dimensions().second - 1)
        right_cell = m_grid.at(x).at(y + 1);


    //If left block exists and hasn't been collapsed yet, remove all cells that can't have state
    //as their right (According to the loaded rules)
    if(left_cell != nullptr && !(left_cell -> is_collapsed()))
    {
        auto left_allowed = get_left_allowed(state);
        left_cell ->set_available_states(left_allowed);
    }

    //If right block exists and hasn't been collapsed yet, remove all cells that can't have state a
    //as their left (according to the loaded rules)
    if(right_cell != nullptr && !(right_cell -> is_collapsed()))
    {
        auto right_allowed = get_right_allowed(state);
        right_cell ->set_available_states(right_allowed);
    }
}

//When we collapse a cell into a specific current_state, the cell to it's left can't take certain states anymore
//and these are defined from the loaded rules.
//Since it constraints cells to it's left, this makes it the right tile (in the rules)
std::vector<CellState *> TiledModel_ImageGrid::get_left_allowed(CellState* current_state)
{
    std::vector<CellState*> allowed_states;

    for(auto rule : m_rules)
    {
        //Find all rules where this current_state is the right neighbour and get their left neighbour
        if(rule->m_right == current_state -> get_name())
        {
            for(auto state : m_all_states)
            {
                if(state -> get_name() == rule-> m_left)
                {
                    allowed_states.emplace_back(state);
                }
            }
        }
    }
    return allowed_states;
}


//When we collapse a cell into a specific current_state, the cell to it's right can't take certain states anymore
//and these are defined from the loaded rules
//Since it constraints cells to it's right, this makes it the left tile (in the rules)
std::vector<CellState*> TiledModel_ImageGrid::get_right_allowed(CellState *current_state)
{
    std::vector<CellState*> allowed_states;
    for(auto rule : m_rules)
    {
        //Find all rules where this current_state is the left neighbour and get their right neighbour
        if(rule -> m_left == current_state -> get_name())
        {
            for(auto state : m_all_states)
            {
                if(state -> get_name() == rule -> m_right)
                {
                    allowed_states.emplace_back(state);
                }
            }
        }
    }
    return allowed_states;
}