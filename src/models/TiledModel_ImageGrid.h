//
// Created by User on 2023/08/22.
//

#ifndef WAVE_FUNCTION_COLLAPSE_UI_TILEDMODEL_IMAGEGRID_H
#define WAVE_FUNCTION_COLLAPSE_UI_TILEDMODEL_IMAGEGRID_H

#include <map>
#include <QBitmap>
#include <QObject>
#include "Cell.h"
#include "TiledModelRule.h"
#include <QDomDocument>

class TiledModel_ImageGrid : public QObject
{
    Q_OBJECT
public:
    TiledModel_ImageGrid(int width, int height, const std::string& image_dir);
    const std::pair<int, int>& dimensions();
    std::vector<std::vector<Cell*>> grid();
    std::map<std::string, QPixmap*> get_name_image_mapping();
    void init_generation();
    Cell* get_initial_cell();
    void collapse(Cell* next_cell);
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
private:
    std::vector<std::vector<Cell*>> m_grid;
    std::pair<int, int> m_dimensions;
    std::map<std::string, QPixmap*> m_name_image_map;
    std::vector<CellState*> m_all_states;
    QDomDocument* m_tile_set_doc;
    std::vector<TiledModelRule*> m_rules;
    std::vector<std::vector<std::string>> m_rules_matrix;
    Cell* m_initial_cell;
    Cell* m_current_collapsed;
};
#endif //WAVE_FUNCTION_COLLAPSE_UI_TILEDMODEL_IMAGEGRID_H
