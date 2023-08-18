//
// Created by User on 2023/08/14.
//

#include "SudokuScene.h"
#include "../utils.h"
#include "../widgets/Button.h"

SudokuScene::SudokuScene(const std::string& filename, QWidget* parent) : AbstractScene(parent)
{
    //Generate a random number between 1 and 180 (QGradient::Present have values from 1 to 180)
    auto rand_gradient = Utils::generate_random_int(1, 180);
    auto gradient = QGradient(QGradient::Preset(rand_gradient));
    auto brush = QBrush(gradient);
    this ->setBackgroundBrush(brush);

    m_sudoku_grid = new BoardContainer;
    m_board = new SudokuBoard(filename);
    init();
}

void SudokuScene::animate()
{
    m_board -> init_solve();
    m_board -> solve();
}

/**
 * Initializes the scene UI by mapping props from the model to the UI GridTile and
 * adds them to the scene
 */
void SudokuScene::init()
{
    const auto& initial_board = m_board -> get_current_board();
    auto board_size = m_sudoku_grid -> getSize();

    for(auto row = 0; row < board_size; row++)
    {
        std::vector<GridTile*> _row;
        for(auto col = 0; col < board_size; col++)
        {
            auto x = GridTile::TILE_SIZE * row;
            auto y = GridTile::TILE_SIZE * col;
            const auto& block_model = initial_board.at(col).at(row);
            auto value = block_model -> get_collapsed_state() -> get_value();
            auto is_permanently_collapsed = block_model -> get_is_permanently_collapsed();
            auto tile = new GridTile(QString::number(value), x, y, is_permanently_collapsed);
            this ->addItem(tile);
            _row.emplace_back(tile);
        }
        m_grid_ui.emplace_back(_row);
    }
    arrangeItems();
}

void SudokuScene::arrangeItems()
{
    const int SQUARE_GRID_SIZE = 3;
    auto grid_layout_index = 0;
    auto grid_row_index = 0;
    auto grid_col_index = 0;

    auto inner_grid_layouts = m_sudoku_grid -> getInnerGridLayouts();
    auto inner_grid_widgets = m_sudoku_grid -> getInnerGridWidgets();
    auto board_size = m_sudoku_grid -> getSize();

    for(auto row = 0; row < board_size; row += SQUARE_GRID_SIZE)
    {
        for(auto col = 0; col < board_size; col += SQUARE_GRID_SIZE)
        {
            const auto& layout = inner_grid_layouts.at(grid_layout_index);
            for(auto m = 0; m < SQUARE_GRID_SIZE; m++)
            {
                for(auto n = 0; n < SQUARE_GRID_SIZE; n++)
                {
                    auto tile = m_grid_ui.at(m + row).at(n + col);
                    layout->addItem(tile, m, n);
                }
            }
            m_sudoku_grid ->add(layout, grid_row_index, grid_col_index);
            const auto& widget = inner_grid_widgets.at(grid_layout_index);
            this ->addItem(widget);

            grid_col_index++;
            grid_layout_index++;
        }
        grid_col_index = 0;
        grid_row_index++;
    }
}
