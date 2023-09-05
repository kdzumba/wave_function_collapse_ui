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

TiledModel_ImageGrid::TiledModel_ImageGrid(int number_of_rows, int number_of_cols, const std::string& image_dir): m_generator(std::random_device{}())
{
    m_dimensions = std::make_pair(number_of_rows, number_of_cols);
    m_tile_set_doc = new QDomDocument;
    m_wave_state = new WaveState;
    m_in_contradiction = false;
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
            cell->set_superposition(m_all_states);
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


void TiledModel_ImageGrid::observe()
{
    auto next_cell = get_min_entropy();
    if(next_cell == nullptr)
    {
        m_in_contradiction = true;
        std::cout << "No least entropy cell found" << std::endl;
        return;
    }
    //Need to factor in pattern frequencies (weights) before selecting next_state
    double weight_sum = 0;
    std::cout << "Next Cell: " << next_cell << std::endl;
    std::cout << "Next Cell Position: " << next_cell->get_position().first << " " << next_cell->get_position().second << std::endl;
    std::cout << "Next Cell Available States Count: " << next_cell->get_superposition().size() << std::endl;
    std::cout << "Wave states possible states for cell: " << m_wave_state->number_of_patterns.at(next_cell->get_position().first).at(next_cell->get_position().second) << std::endl;

    for(const auto& state : next_cell->get_superposition())
    {
        weight_sum += state -> get_weight();
    }
    auto rand_value = Utils::generate_random_double(0, weight_sum);

    CellState* next_state = nullptr;

    for(auto k = 0; k < next_cell->get_superposition().size(); k++)
    {
        rand_value -= next_cell->get_superposition().at(k)->get_weight();
        if(rand_value <= 0)
        {
            next_state = next_cell->get_superposition().at(k);
            break;
        }
    }

    if(next_cell -> get_state() == next_state)
        return;

    //Update wave state

    auto x = next_cell -> get_position().first;
    auto y = next_cell -> get_position().second;

    for(const auto& element : next_cell -> get_superposition())
    {
        next_cell->remove_state(element);
        update_wave_state(x, y, element->get_name());
    }
    next_cell -> set_collapsed_state(next_state);
    m_propagating.emplace_back(x, y, next_state->get_name());
}

bool TiledModel_ImageGrid::is_fully_generated() const {
    return m_in_contradiction;
}

void TiledModel_ImageGrid::generate()
{
    while(true)
    {
        observe();
        propagate_collapse_info();
    }
}

void TiledModel_ImageGrid::reset()
{
    for(const auto& row : m_wave)
    {
        for(auto cell : row)
        {
            cell->set_superposition(m_all_states);
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
            m_name_cardinality_symmetry_map.insert({name, std::make_tuple(tile_cardinality, symmetry)});

            for(int i = 0; i < tile_cardinality; i++)
            {
                auto state_name = i == 0 ? name  : name + " " + std::to_string(i);
                m_all_states.emplace_back(new CellState(tile_index, state_name, symmetry, i, weight_value));

                m_pattern_weights.emplace_back(weight_value);
                tile_index++;
            }
            tile = tile.nextSiblingElement("tile");
        }

        Utils::normalize(m_pattern_weights);
        m_plogp_pattern_weights = Utils::get_plogp(m_pattern_weights);

        auto neighbours_element = tiles_element.nextSibling();
        read_neighbour_rules((neighbours_element));
    }
    load_state_images(images_path);
}

std::map<std::string, QPixmap *> TiledModel_ImageGrid::get_name_image_mapping()
{
    return m_name_image_map;
}

std::vector<CellState *> TiledModel_ImageGrid::get_left_allowed(const std::string& state_name)
{
    std::vector<CellState*> allowed_states;

    for(auto rule : m_rules)
    {
        //Find all rules where this current_state is the right neighbour and get their left neighbour
        if(rule->m_rule.at(Direction::RIGHT) == state_name)
        {
            for(auto state : m_all_states)
            {
                if(state -> get_name() == rule-> m_rule.at(Direction::LEFT))
                {
                    allowed_states.emplace_back(state);
                }
            }
        }
    }
    return allowed_states;
}

std::vector<CellState*> TiledModel_ImageGrid::get_right_allowed(const std::string& state_name)
{
    std::vector<CellState*> allowed_states;
    for(auto rule : m_rules)
    {
        //Find all rules where this current_state is the left neighbour and get their right neighbour
        if(rule -> m_rule.at(Direction::LEFT) == state_name)
        {
            for(auto state : m_all_states)
            {
                if(state -> get_name() == rule -> m_rule.at(Direction::RIGHT))
                {
                    allowed_states.emplace_back(state);
                }
            }
        }
    }
    return allowed_states;
}

void TiledModel_ImageGrid::read_neighbour_rules(const QDomNode& neighbours_node)
{
    auto neighbour = neighbours_node.firstChildElement("neighbor");
    while(!neighbour.isNull())
    {
        auto left = neighbour.attribute("left");
        auto right = neighbour.attribute("right");
        generate_and_add_rule(left, right);

        neighbour = neighbour.nextSiblingElement("neighbor");
    }

    unsigned rotation_group = 0;
    for(const auto& rule : m_rules)
    {
        if(rotation_group >= 8)
        {
            std::cout << std::endl;
            rotation_group = 0;
        }

        if(!rule->m_rule.at(Direction::LEFT).empty())
            std::cout << "LEFT: " << rule->m_rule.at(Direction::LEFT);
        if(!rule->m_rule.at(Direction::RIGHT).empty())
            std::cout << " " << "RIGHT: " << rule->m_rule.at(Direction::RIGHT);

        if(!rule->m_rule.at(Direction::UP).empty())
            std::cout << "UP: " << rule->m_rule.at(Direction::UP);
        if(!rule->m_rule.at(Direction::DOWN).empty())
            std::cout << " DOWN: " << rule->m_rule.at(Direction::DOWN);
        std::cout << std::endl;
        rotation_group++;
    }

    generate_propagator_states();
}

std::vector<CellState *> TiledModel_ImageGrid::get_up_allowed(const std::string& state_name)
{
    std::vector<CellState*> allowed_states;
    for(auto rule : m_rules)
    {
        if(rule -> m_rule.at(Direction::DOWN) == state_name)
        {
            for(auto state : m_all_states)
            {
                if(state -> get_name() == rule -> m_rule.at(Direction::UP))
                {
                    allowed_states.emplace_back(state);
                }
            }
        }
    }
    return allowed_states;
}

std::vector<CellState *> TiledModel_ImageGrid::get_down_allowed(const std::string& state_name)
{
    std::vector<CellState*> allowed_states;
    for(auto rule : m_rules)
    {
        if(rule -> m_rule.at(Direction::UP) == state_name)
        {
            for(auto state : m_all_states)
            {
                if(state -> get_name() == rule -> m_rule.at(Direction::DOWN))
                {
                    allowed_states.emplace_back(state);
                }
            }
        }
    }
    return allowed_states;
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
        m_wave_state->number_of_patterns.emplace_back(m_dimensions.second, m_all_states.size());
        auto entropies = std::vector<double>(dimensions().second, entropy_base);
        m_wave_state->entropy.emplace_back(entropies);
        auto weights_sum = std::vector<double>(m_dimensions.second, base_s);
        m_wave_state->weights_sum.emplace_back(weights_sum);
    }
    m_min_abs_half_plogp = Utils::get_min_abs_half(m_plogp_pattern_weights);
}

Cell* TiledModel_ImageGrid::get_min_entropy()
{
    std::uniform_real_distribution<> distribution(0, m_min_abs_half_plogp);
    double min = std::numeric_limits<double>::infinity();
    Cell* min_entropy_cell = nullptr;

    for(unsigned i = 0; i < dimensions().first; i++)
    {
        for(unsigned j = 0; j < dimensions().second; j++)
        {
            //This cell has already collapsed into a single state and we don't want to consider it for entropy
            if(m_wave_state->number_of_patterns.at(i).at(j) == 0)
                continue;

            double entropy = m_wave_state->entropy.at(i).at(j);
            if(entropy <= min)
            {
                double noise = distribution(this->m_generator);
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

void TiledModel_ImageGrid::generate_propagator_states()
{
    for(const auto& state : m_all_states)
    {
        const auto& state_name = state->get_name();
        auto left_compatible = get_left_allowed(state_name);
        auto right_compatible = get_right_allowed(state_name);
        auto up_compatible = get_up_allowed(state_name);
        auto down_compatible = get_down_allowed(state_name);

        std::array<std::vector<CellState*>, 4> state_compatible = {};
        state_compatible[0] = up_compatible;
        state_compatible[1] = left_compatible;
        state_compatible[2] = down_compatible;
        state_compatible[3] = right_compatible;

        m_propagator_state.insert({state_name, state_compatible});
    }
}

void TiledModel_ImageGrid::update_wave_state(int x, int y, const std::string& next_state_name)
{
    unsigned state_index = 0;

    for(unsigned index = 0; index < m_all_states.size(); index++)
    {
        if(m_all_states.at(index)->get_name() == next_state_name)
        {
            state_index = index;
            break;
        }
    }

    m_wave_state->plogp_weights_sum[x][y]-= m_plogp_pattern_weights.at(state_index);
    m_wave_state->weights_sum[x][y] -= m_pattern_weights.at(state_index);
    m_wave_state->log_weights_sum[x][y] = log(m_wave_state->weights_sum[x][y]);
    m_wave_state->number_of_patterns[x][y]--;
    m_wave_state->entropy[x][y] = m_wave_state->log_weights_sum.at(x).at(y) - m_wave_state->plogp_weights_sum.at(x).at(y) / m_wave_state->weights_sum.at(x).at(y);
}

void TiledModel_ImageGrid::propagate_collapse_info()
{
    while(!m_propagating.empty())
    {
        std::cout << "m_propagating size: " << m_propagating.size() << std::endl;
        int x, y;
        std::string state_name;
        std::tie(x, y, state_name) = m_propagating.back();

        //Pop the info off the stack once read
        m_propagating.pop_back();

        //We want to propagate this information in all 4 directions that are neighbours to the current cell
        for(unsigned direction = 0; direction < 4; direction++)
        {
            int dx = directions_x[direction];
            int dy = directions_y[direction];

            int x2 = x + dx;
            int y2 = y + dy;

            if(x2 < 0 || x2 >= dimensions().first)
                continue;

            if(y2 < 0 || y2 >= dimensions().second)
                continue;

            //The next cell in direction=direction and states that are compatible to that location
            auto next_cell = m_wave.at(x2).at(y2);
            if(next_cell->is_collapsed())
                continue;

            //These are the states still possible in direction for next_cell
            const auto& patterns = m_propagator_state.at(state_name)[direction];
            if(patterns.empty()) {
//                m_in_contradiction = true;
                std::cout << "CONTRADICTION: " << state_name << ": " << direction << std::endl;
//                return;
            }

            //Let's remove all states that aren't possible anymore for next_cell and put them in elements to remove
            for(const auto& state : next_cell->get_superposition())
            {
                if(std::find(patterns.begin(), patterns.end(), state) == patterns.end())
                {
                    next_cell->remove_state(state);
                    update_wave_state(x2, y2, state->get_name());
                }
                else
                {
                    continue;
                }
            }

            std::cout << "Done removing from next cell in direction : " << direction << std::endl;
        }
    }
}

std::tuple<std::string, int> TiledModel_ImageGrid::get_name_orientation(const QString &rule)
{
    auto name_orientation_list = rule.split(" ");
    auto name = name_orientation_list.first().toStdString();
    auto orientation = name_orientation_list.last().toInt();
    return {name, orientation};
}

void TiledModel_ImageGrid::generate_and_add_rule(const QString& left, const QString& right)
{
    std::string left_name, right_name;
    int left_orientation, right_orientation;
    std::tie(left_name, left_orientation) = get_name_orientation(left);
    std::tie(right_name, right_orientation) = get_name_orientation(right);

    int left_cardinality, right_cardinality;
    std::string right_symmetry, left_symmetry;
    right_symmetry = std::get<1>(m_name_cardinality_symmetry_map.at(right_name));
    left_symmetry = std::get<1>(m_name_cardinality_symmetry_map.at(left_name));

    right_cardinality = std::get<0>(m_name_cardinality_symmetry_map.at(right_name));
    left_cardinality = std::get<0>(m_name_cardinality_symmetry_map.at(left_name));

    auto append_orientation = [&](const std::string& name, int orientation) -> std::string{
        if(orientation == 0)
            return name;
        else
            return name + " " + std::to_string(orientation);
    };

    //Add left-right rule as we read it from the specification
    auto left_read = append_orientation(left_name, left_orientation);
    auto right_read = append_orientation(right_name, right_orientation);
    auto rule = new TiledRuleModel;
    rule->m_rule.insert({Direction::LEFT, left_read});
    rule->m_rule.insert({Direction::RIGHT, right_read});
    rule->m_rule.insert({Direction::UP, ""});
    rule->m_rule.insert({Direction::DOWN, ""});
    m_rules.emplace_back(rule);

    auto read_reflection_pair = get_reflection_pair(left_name, right_name, left_orientation,
                                          right_orientation, left_symmetry, right_symmetry, Axis::X);
    auto read_reflection_rule = new TiledRuleModel;
    read_reflection_rule->m_rule.insert({Direction::LEFT, read_reflection_pair.first});
    read_reflection_rule->m_rule.insert({Direction::RIGHT, read_reflection_pair.second});
    read_reflection_rule->m_rule.insert({Direction::UP, ""});
    read_reflection_rule->m_rule.insert({Direction::DOWN, ""});
    m_rules.emplace_back(read_reflection_rule);
    //If both tiles can be rotated, rotate to get up-down rule
    auto rotation_count = 1;

    while(rotation_count < 4)
    {
        std::string right_rotated = right_name;
        std::string left_rotated = left_name;

        if(right_symmetry != "X")
        {
            right_rotated = append_orientation(right_name, (++right_orientation) % right_cardinality);
        }

        if(left_symmetry != "X")
        {
            left_rotated = append_orientation(left_name, (++left_orientation) % left_cardinality);
        }

        auto try_insert_rule = [&](TiledRuleModel* rule) -> void {
            auto found_iter = std::find_if(m_rules.begin(), m_rules.end(), [&](TiledRuleModel* r) -> bool
            {
                return r->m_rule.at(Direction::LEFT) == rule->m_rule.at(Direction::LEFT) && r->m_rule.at(Direction::RIGHT) == rule->m_rule.at(Direction::RIGHT)
                && r->m_rule.at(Direction::DOWN) == rule->m_rule.at(Direction::DOWN) && r->m_rule.at(Direction::UP) == rule->m_rule.at(Direction::UP);
            });
            if(found_iter != m_rules.end())
                return;
            m_rules.emplace_back(rule);
        };

        auto sub_rule = new TiledRuleModel;
        auto reflection_rule = new TiledRuleModel;
        std::pair<std::string, std::string> reflection_pair;

        switch(rotation_count)
        {
            case 1:
                sub_rule->m_rule.insert({Direction::UP, right_rotated});
                sub_rule->m_rule.insert({Direction::DOWN, left_rotated});
                sub_rule->m_rule.insert({Direction::LEFT, ""});
                sub_rule->m_rule.insert({Direction::RIGHT, ""});
                try_insert_rule(sub_rule);

                reflection_pair = get_reflection_pair(left_name, right_name, left_orientation,
                                                      right_orientation, left_symmetry, right_symmetry, Axis::X);
                reflection_rule->m_rule.insert({Direction::UP, reflection_pair.first});
                reflection_rule->m_rule.insert({Direction::DOWN, reflection_pair.second});
                reflection_rule->m_rule.insert({Direction::LEFT, ""});
                reflection_rule->m_rule.insert({Direction::RIGHT, ""});
                try_insert_rule(reflection_rule);
                break;
            case 2:
                sub_rule->m_rule.insert({Direction::LEFT, right_rotated});
                sub_rule->m_rule.insert({Direction::RIGHT, left_rotated});
                sub_rule->m_rule.insert({Direction::UP, ""});
                sub_rule->m_rule.insert({Direction::DOWN, ""});
                try_insert_rule(sub_rule);

                reflection_pair = get_reflection_pair(left_name, right_name, left_orientation,
                                                      right_orientation, left_symmetry, right_symmetry, Axis::Y);
                reflection_rule->m_rule.insert({Direction::UP, ""});
                reflection_rule->m_rule.insert({Direction::DOWN, ""});
                reflection_rule->m_rule.insert({Direction::LEFT, reflection_pair.first});
                reflection_rule->m_rule.insert({Direction::RIGHT, reflection_pair.second});
                try_insert_rule(reflection_rule);
                break;
            case 3:
                sub_rule->m_rule.insert({Direction::UP, left_rotated});
                sub_rule->m_rule.insert({Direction::DOWN, right_rotated});
                sub_rule->m_rule.insert({Direction::LEFT, ""});
                sub_rule->m_rule.insert({Direction::RIGHT, ""});
                try_insert_rule(sub_rule);

                reflection_pair = get_reflection_pair(left_name, right_name, left_orientation,
                                                      right_orientation, left_symmetry, right_symmetry, Axis::X);
                reflection_rule->m_rule.insert({Direction::UP, reflection_pair.first});
                reflection_rule->m_rule.insert({Direction::DOWN, reflection_pair.second});
                reflection_rule->m_rule.insert({Direction::LEFT, ""});
                reflection_rule->m_rule.insert({Direction::RIGHT, ""});
                try_insert_rule(reflection_rule);
                break;
            default:
                std::cout << "Unexpected rotation" << std::endl;
        }
        rotation_count++;
    }
}

bool TiledModel_ImageGrid::in_contradiction() const {
    return m_in_contradiction;
}

std::pair<std::string, std::string>
TiledModel_ImageGrid::get_reflection_pair(const std::string &first, const std::string &second, int first_orientation,
                                          int second_orientation, const std::string &first_symmetry,
                                          const std::string &second_symmetry, Axis axis) {

    auto append_orientation = [&](const std::string& name, int orientation) -> std::string{
        if(orientation == 0)
            return name;
        else
            return name + " " + std::to_string(orientation);
    };

    std::string first_;
    std::string second_;
    auto is_even = [=](int orientation) -> bool{return orientation % 2 == 0; };

    switch (axis)
    {
        //This is horrible, but I don't have time to think about it right now
        case Axis::X:
            if(first_symmetry == "I" || first_symmetry == "X")
            {
                first_ = append_orientation(first, first_orientation);
            }
            else if(first_symmetry == "T" && is_even(first_orientation))
            {
                first_ = append_orientation(first, (first_orientation + 2) % 4);
            }
            else if(first_symmetry == "T" && !is_even(first_orientation))
            {
                first_ = append_orientation(first, (first_orientation));
            }
            else if(first_symmetry == "L" && is_even(first_orientation))
            {
                first_ = append_orientation(first, (first_orientation + 3) % 4);
            }
            else if(first_symmetry == "L" && !is_even(first_orientation))
            {
                first_ = append_orientation(first, (first_orientation + 1) % 4);
            }

            if(second_symmetry == "I" || second_symmetry == "X")
            {
                second_ = append_orientation(second, second_orientation);
            }
            else if(first_symmetry == "T" && is_even(second_orientation))
            {
                second_ = append_orientation(second, (second_orientation + 2) % 4);
            }
            else if(first_symmetry == "T" && !is_even(second_orientation))
            {
                second_ = append_orientation(second, (second_orientation));
            }
            else if(first_symmetry == "L" && is_even(second_orientation))
            {
                second_ = append_orientation(second, (second_orientation + 3) % 4);
            }
            else if(first_symmetry == "L" && !is_even(second_orientation))
            {
                second_ = append_orientation(second, (second_orientation + 1) % 4);
            }
            std::cout << "Original Left: " << append_orientation(first, first_orientation) << " Original Right:" << append_orientation(second, second_orientation) << std::endl;
            std::cout << "Reflected Left: " << second_ << " Reflected Right: " << first_ << std::endl;
            return std::make_pair(second_, first_);

        case Axis::Y:
            if(first_symmetry == "I" || first_symmetry == "X")
            {
                first_ = append_orientation(first, first_orientation);
            }
            else if(first_symmetry == "T" && is_even(first_orientation))
            {
                first_ = append_orientation(first, first_orientation);
            }
            else if(first_symmetry == "T" && !is_even(first_orientation))
            {
                first_ = append_orientation(first, (first_orientation + 2) % 4);
            }
            else if(first_symmetry == "L" && is_even(first_orientation))
            {
                first_ = append_orientation(first, (first_orientation + 1) % 4);
            }
            else if(first_symmetry == "L" && !is_even(first_orientation))
            {
                first_ = append_orientation(first, (first_orientation + 3) % 4);
            }

            if(second_symmetry == "I" || second_symmetry == "X")
            {
                second_ = append_orientation(second, second_orientation);
            }
            else if(second_symmetry == "T" && is_even(second_orientation))
            {
                second_ = append_orientation(second, second_orientation);
            }
            else if(second_symmetry == "T" && !is_even(second_orientation))
            {
                second_ = append_orientation(second, (second_orientation + 2) % 4);
            }
            else if(second_symmetry == "L" && is_even(second_orientation))
            {
                second_ = append_orientation(second, (second_orientation + 1) % 4);
            }
            else if(second_symmetry == "L" && !is_even(second_orientation))
            {
                second_ = append_orientation(second, (second_orientation + 3) % 4);
            }
            std::cout << "Original Up: " << append_orientation(first, first_orientation) << " Original Down:" << append_orientation(second, second_orientation) << std::endl;
            std::cout << "Reflected Up: " << second_ << " Reflected Down: " << first_ << std::endl;
            return std::make_pair(second_, first_);
    }
    return std::make_pair(second_, first_);
}



