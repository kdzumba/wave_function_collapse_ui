//
// Created by User on 2023/08/22.
//

#ifndef WAVE_FUNCTION_COLLAPSE_UI_TILEDMODEL_IMAGEGRID_H
#define WAVE_FUNCTION_COLLAPSE_UI_TILEDMODEL_IMAGEGRID_H

#include <map>
#include <QBitmap>
#include <QObject>
#include "Cell.h"
#include "TiledRuleModel.h"
#include <QDomDocument>
#include <random>

class WaveState
{
public:
    std::vector<std::vector<double>> plogp_weights_sum;
    std::vector<std::vector<double>> weights_sum;
    std::vector<std::vector<double>> log_weights_sum;
    std::vector<std::vector<unsigned>> number_of_patterns;
    std::vector<std::vector<double>> entropy;
};

constexpr int directions_x[4] = {0, -1, 1, 0};
constexpr int directions_y[4] = {-1, 0, 0, 1};

constexpr unsigned get_opposite_direction(unsigned direction)
{
    return 3 - direction;
}

class TiledModel_ImageGrid : public QObject
{
    Q_OBJECT
    using PropagatorState = std::map<std::string, std::array<std::vector<CellState*>, 4>>;
public:
    TiledModel_ImageGrid(int number_of_rows, int number_of_cols, const std::string& image_dir);
    const std::pair<int, int>& dimensions();
    std::vector<std::vector<Cell*>> grid();
    std::map<std::string, QPixmap*> get_name_image_mapping();
    void observe();
    bool is_fully_generated() const;
    void generate();
    void reset();
private:
    void load_state_images(const std::string& img_directory);
    void load_tile_set_specification(const std::string& spec_file, const std::string& images_path);
    void propagate_collapse_info();
    std::vector<CellState*> get_left_allowed(const std::string& state_name);
    std::vector<CellState*> get_right_allowed(const std::string& state_name);
    std::vector<CellState*> get_up_allowed(const std::string& state_name);
    std::vector<CellState*> get_down_allowed(const std::string& state_name);
    void generate_propagator_states();
    void update_wave_state(int x, int y, CellState* next_state);
    void read_neighbour_rules(const QDomNode& neighbours_node);
    void calculate_initial_entropy();
    static std::tuple<std::string, int> get_name_orientation(const QString& rule);
    Cell* get_min_entropy();
    void generate_and_add_rule(const QString& left, const QString& right);
private:
    std::vector<std::vector<Cell*>> m_wave;
    std::pair<int, int> m_dimensions;
    std::map<std::string, QPixmap*> m_name_image_map;
    std::vector<CellState*> m_all_states;
    QDomDocument* m_tile_set_doc;
    std::vector<TiledRuleModel*> m_rules;
    std::map<std::string, std::tuple<int, std::string>> m_name_cardinality_symmetry_map;
    std::vector<double> m_pattern_weights;
    std::vector<double> m_plogp_pattern_weights;
    double m_min_abs_half_plogp;
    WaveState* m_wave_state;
    std::minstd_rand m_generator;
    PropagatorState m_propagator_state;
    std::vector<std::tuple<int, int, std::string>> m_propagating;
    bool m_in_contradiction;
};
#endif //WAVE_FUNCTION_COLLAPSE_UI_TILEDMODEL_IMAGEGRID_H
