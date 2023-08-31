//
// Created by User on 2023/08/22.
//

#include <filesystem>
#include <QDomDocument>
#include <QFile>
#include <iostream>
#include <random>
#include "TiledModel_ImageGrid.h"
#include "../utils.h"

TiledModel_ImageGrid::TiledModel_ImageGrid(int number_of_rows, int number_of_cols, const std::string& image_dir): generator(1)
{
    m_dimensions = std::make_pair(number_of_rows, number_of_cols);
    m_tile_set_doc = new QDomDocument;
    m_initial_cell = nullptr;
    m_current_collapsed = nullptr;
    m_wave_state = new WaveState;
    //Load the tileset and it's defined constraints
    load_tile_set_specification("tilesets/Circuit.xml", image_dir);
    calculate_initial_entropy();

    for(auto row = 0; row < number_of_rows; row++)
    {
        auto row_ = std::vector<Cell*>();
        for(auto col = 0; col < number_of_cols; col++)
        {
            auto cell = new Cell(row, col);
            //Initially, all cells "are" every possible state at the same time
            cell ->set_available_states(m_all_states);
            row_.emplace_back(cell);
        }
        m_wave.emplace_back(row_);
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
    return m_wave;
}

const std::pair<int, int> &TiledModel_ImageGrid::dimensions()
{
    return m_dimensions;
}

void TiledModel_ImageGrid::init_generation()
{
    observe();
}

Cell *TiledModel_ImageGrid::get_initial_cell()
{
    std::vector<std::pair<int,int>> positions_to_consider;

    for(const auto& row : m_wave)
    {
        for(const auto& cell : row)
        {
            positions_to_consider.emplace_back(cell->get_position());
        }
    }
    auto rand_index = Utils::generate_random_int(0, (int) positions_to_consider.size());
    auto initial_coordinate = positions_to_consider.at(rand_index);
    return m_wave.at(initial_coordinate.first).at(initial_coordinate.second);
}

void TiledModel_ImageGrid::observe()
{
    auto next_cell = get_min_entropy(generator);
    //Need to factor in pattern frequencies (weights) before selecting next_state
    double weight_sum = 0;
    for(const auto& state : next_cell -> get_available_states())
    {
        weight_sum += state -> get_weight();
    }
    auto rand_value = Utils::generate_random_double(0, weight_sum);

    CellState* next_state = nullptr;

    for(auto k = 0; k < next_cell -> get_available_states().size(); k++)
    {
        rand_value -= next_cell -> get_available_states().at(k)->get_weight();
        if(rand_value <= 0)
        {
            next_state = next_cell -> get_available_states().at(k);
            break;
        }
    }

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
    print_entropies();
    m_current_collapsed = next_cell;
}

bool TiledModel_ImageGrid::is_fully_generated() {
    for(const auto& row : m_wave)
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
    observe();
    if(!(is_fully_generated()))
        generate();
}

Cell *TiledModel_ImageGrid::least_entropy_cell()
{
    std::map<unsigned int, std::vector<Cell*>> entropy_to_cell_map;
    auto possible_cells = std::vector<Cell*>();

    for(const auto& row : m_wave)
    {
        for(const auto& cell : row)
        {
            if(!(cell -> is_collapsed()))
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
    }
    auto min_entropy=  entropy_to_cell_map.begin() -> second;
    if(min_entropy.empty())
        std::cout << "No more least entropies " << std::endl;

    auto rand_index = Utils::generate_random_int(0, (int) min_entropy.size() - 1);
    return min_entropy.at(rand_index);
}

void TiledModel_ImageGrid::reset()
{
    for(const auto& row : m_wave)
    {
        for(auto cell : row)
        {
            cell ->set_available_states(m_all_states);
            cell -> reset();
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
            auto weight = tile.attribute("weight").toStdString();
            auto weight_value = weight.empty() ? 1 : std::stod(weight);

            auto tile_cardinality = CellState::symmetry_cardinality_map.at(symmetry);
            m_name_cardinality_map.insert({name, tile_cardinality});

            for(int i = 0; i < tile_cardinality; i++)
            {
                auto state_name = i == 0 ? name  : name + " " + std::to_string(i);
                m_all_states.emplace_back(new CellState(tile_index, state_name, symmetry, i, weight_value));

                m_pattern_weights.emplace_back(weight_value);
                m_plogp_pattern_weights = Utils::get_plogp(m_pattern_weights);
                tile_index++;
            }
            tile = tile.nextSiblingElement("tile");
        }

        auto neighbours_element = tiles_element.nextSibling();
        read_neighbour_rules((neighbours_element));
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
    Cell* up_cell= nullptr;
    Cell* down_cell = nullptr;

    //0th column doesn't have a left cell neighbour
    if(y > 0)
        left_cell = m_wave.at(x).at(y - 1);

    //Last cell in a row doesn't have a right cell neighbour
    if(y < dimensions().second - 1)
        right_cell = m_wave.at(x).at(y + 1);

    if(x > 0)
        up_cell = m_wave.at(x - 1).at(y);

    if(x < dimensions().first - 1)
        down_cell = m_wave.at(x + 1).at(y);


    //If left block exists and hasn't been collapsed yet, remove all cells that can't have state
    //as their right (According to the loaded rules)
    if(left_cell != nullptr && !(left_cell -> is_collapsed()))
    {
        auto left_allowed = get_left_allowed(state);
        left_cell ->set_available_states(left_allowed);
    }

    if(up_cell != nullptr && !(up_cell -> is_collapsed()))
    {
        auto up_allowed = get_up_allowed(state);
        up_cell ->set_available_states(up_allowed);
    }

    if(down_cell != nullptr && !(down_cell -> is_collapsed()))
    {
        auto down_allowed = get_down_allowed(state);
        down_cell ->set_available_states(down_allowed);
    }

    //If right block exists and hasn't been collapsed yet, remove all cells that can't have state a
    //as their left (according to the loaded rules)
    if(right_cell != nullptr && !(right_cell -> is_collapsed()))
    {
        auto right_allowed = get_right_allowed(state);
        right_cell ->set_available_states(right_allowed);
    }
}

//When we reset a cell into a specific current_state, the cell to it's left can't take certain states anymore
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


//When we reset a cell into a specific current_state, the cell to it's right can't take certain states anymore
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

void TiledModel_ImageGrid::read_neighbour_rules(QDomNode neighbours_node)
{
    auto neighbour = neighbours_node.firstChildElement("neighbor");
    while(!neighbour.isNull())
    {
        auto left = neighbour.attribute("left");
        auto left_name_orientation_list = left.split(" ");
        auto left_name = left_name_orientation_list.first().toStdString();
        auto left_orientation = left_name_orientation_list.last().toInt();

        auto right = neighbour.attribute("right");
        auto right_name_orientation_list = right.split(" ");
        auto right_name = right_name_orientation_list.first().toStdString();
        auto right_orientation = right_name_orientation_list.last().toInt();

        if(left_orientation > 0)
            left_name = left_name.append(" " ).append(std::to_string(left_orientation));
        if(right_orientation > 0)
            right_name = right_name.append(" ").append(std::to_string(right_orientation));

        auto main_rule_model = new TiledRuleModel(left_name, right_name, "", "");
        m_rules.emplace_back(main_rule_model);

        auto left_tile_cardinality = m_name_cardinality_map.at(left_name_orientation_list.first().toStdString());
        auto right_tile_cardinality = m_name_cardinality_map.at(right_name_orientation_list.first().toStdString());

        if(right_orientation + 1 < right_tile_cardinality && left_orientation + 1 < left_tile_cardinality)
        {
            auto up = right_name_orientation_list.first().toStdString() + " " + std::to_string(++right_orientation);
            auto down = left_name_orientation_list.first().toStdString() + " " + std::to_string(++left_orientation);
            auto rule_model = new TiledRuleModel("", "", up, down);
            m_rules.emplace_back(rule_model);
        }

        neighbour = neighbour.nextSiblingElement("neighbor");
    }
}

std::vector<CellState *> TiledModel_ImageGrid::get_up_allowed(CellState *current_state)
{
    std::vector<CellState*> allowed_states;
    for(auto rule : m_rules)
    {
        if(rule -> m_down == current_state -> get_name())
        {
            for(auto state : m_all_states)
            {
                if(state -> get_name() == rule -> m_up)
                {
                    allowed_states.emplace_back(state);
                }
            }
        }
    }
    return allowed_states;
}

std::vector<CellState *> TiledModel_ImageGrid::get_down_allowed(CellState *current_state)
{
    std::vector<CellState*> allowed_states;
    for(auto rule : m_rules)
    {
        if(rule -> m_up == current_state -> get_name())
        {
            for(auto state : m_all_states)
            {
                if(state -> get_name() == rule -> m_down)
                {
                    allowed_states.emplace_back(state);
                }
            }
        }
    }
    return allowed_states;
}

void TiledModel_ImageGrid::print_entropies()
{
    for(const auto& row : m_wave)
    {
        for(const auto& cell : row)
        {
            std::cout << cell -> get_entropy() << std::setw(2) << " ";
        }
        std::cout << std::endl;
    }
}

void TiledModel_ImageGrid::calculate_initial_entropy()
{
    double base_entropy = 0;
    double base_s = 0;
    for(unsigned i = 0; i < m_all_states.size(); i++)
    {
        base_entropy += m_plogp_pattern_weights.at(i);
        base_s += m_all_states.at(i)->get_weight();
    }
    double log_base_s = log(base_s);
    double entropy_base = log_base_s - base_entropy / base_s;

    for(unsigned i = 0; i < m_dimensions.first; i++)
    {
        auto plogp_sum = std::vector<double>(m_dimensions.second, base_entropy);
        m_wave_state->plogp_weights_sum.emplace_back(plogp_sum);
        auto log_sum = std::vector<double>(m_dimensions.second, log_base_s);
        m_wave_state->log_weights_sum.emplace_back(log_sum);
        m_wave_state->number_of_patterns.emplace_back(m_all_states.size());
        auto entropies = std::vector<double>(dimensions().second, entropy_base);
        m_wave_state->entropy.emplace_back(entropies);
    }
    m_min_abs_half_plogp = Utils::get_min_abs_half(m_plogp_pattern_weights);
}

Cell* TiledModel_ImageGrid::get_min_entropy(const std::minstd_rand &gen)
{
    std::uniform_real_distribution<> distribution(0, m_min_abs_half_plogp);
    double min = std::numeric_limits<double>::infinity();
    Cell* min_entropy_cell = nullptr;

    for(unsigned i = 0; i < dimensions().first; i++)
    {
        for(unsigned j = 0; j < dimensions().second; j++)
        {
            if(m_wave_state->entropy.at(i).at(j) == 1)
                continue;

            double entropy = m_wave_state->entropy.at(i).at(j);
            if(entropy <= min)
            {
                double noise = distribution(this->generator);
                if(entropy + noise < min)
                {
                    min = entropy + noise;
                    min_entropy_cell = m_wave.at(i).at(j);
                }
            }
        }
    }
    return min_entropy_cell;
}
