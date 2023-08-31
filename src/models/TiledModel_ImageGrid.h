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

class TiledModel_ImageGrid : public QObject
{
    Q_OBJECT
public:
    TiledModel_ImageGrid(int number_of_rows, int number_of_cols, const std::string& image_dir);
    const std::pair<int, int>& dimensions();
    std::vector<std::vector<Cell*>> grid();
    std::map<std::string, QPixmap*> get_name_image_mapping();
    void init_generation();
    Cell* get_initial_cell();
    void observe();
    bool is_fully_generated();
    void generate();
    void reset();
    Cell* least_entropy_cell();
private:
    void load_state_images(const std::string& img_directory);
    void load_tile_set_specification(const std::string& spec_file, const std::string& images_path);
    void propagate_collapse_info(int x, int y, CellState* state);
    std::vector<CellState*> get_left_allowed(CellState* current_state);
    std::vector<CellState*> get_right_allowed(CellState* current_state);
    std::vector<CellState*> get_up_allowed(CellState* current_state);
    std::vector<CellState*> get_down_allowed(CellState* current_state);
    void print_entropies();
    void read_neighbour_rules(QDomNode neighbours_node);
    void calculate_initial_entropy();
    Cell* get_min_entropy(const std::minstd_rand & gen);
private:
    std::vector<std::vector<Cell*>> m_wave;
    std::pair<int, int> m_dimensions;
    std::map<std::string, QPixmap*> m_name_image_map;
    std::vector<CellState*> m_all_states;
    QDomDocument* m_tile_set_doc;
    std::vector<TiledRuleModel*> m_rules;
    Cell* m_initial_cell;
    Cell* m_current_collapsed;
    std::map<std::string, int> m_name_cardinality_map;
    std::vector<double> m_pattern_weights;
    std::vector<double> m_plogp_pattern_weights;
    double m_min_abs_half_plogp;
    WaveState* m_wave_state;
    std::minstd_rand generator;
};
#endif //WAVE_FUNCTION_COLLAPSE_UI_TILEDMODEL_IMAGEGRID_H
