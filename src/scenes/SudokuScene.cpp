//
// Created by User on 2023/08/14.
//

#include <QGraphicsGridLayout>
#include <QGraphicsWidget>
#include "SudokuScene.h"

SudokuScene::SudokuScene(const std::string& filename, QWidget* parent) : AbstractScene(parent)
{
    auto gradient = QGradient(QGradient::JuicyPeach);
    auto brush = QBrush(gradient);
    this ->setBackgroundBrush(brush);
    this ->setSceneRect(0, 0, 1920, 800);

    m_containers = QList<QGraphicsWidget*>();
    m_layouts = QList<QGraphicsGridLayout*>();

    for(auto i = 0; i < BOARD_SIZE; i++)
    {
        auto container = new QGraphicsWidget;
        auto layout = new QGraphicsGridLayout(container);
        m_containers.emplace_back(container);
        m_layouts.emplace_back(layout);
    }

    m_board = new SudokuBoard(filename);
    init();
}

void SudokuScene::animate()
{
    m_board -> init_solve();
    m_board -> solve();
}

void SudokuScene::init()
{
    const auto& initial_board = m_board -> get_current_board();
    for(auto row = 0; row < BOARD_SIZE; row++)
    {
        std::vector<GridTile*> _row;
        for(auto col = 0; col < BOARD_SIZE; col++)
        {
            auto x = GridTile::TILE_SIZE * row;
            auto y = GridTile::TILE_SIZE * col;
            const auto& block_model = initial_board.at(col).at(row);
            auto value = block_model -> get_collapsed_state() -> get_value();
            auto is_permanently_collapsed = block_model -> get_is_permanently_collapsed();
            auto tile = new GridTile(QString::number(value), x, y, is_permanently_collapsed);
            _row.emplace_back(tile);
            this ->addItem(tile);
        }
        m_grid_ui.emplace_back(_row);
    }
    arrangeItems();
}

void SudokuScene::arrangeItems()
{
    const int SQUARE_GRID_SIZE = 3;
    for(auto row = 0; row < SQUARE_GRID_SIZE; row += SQUARE_GRID_SIZE)
    {
        for(auto col = 0; col < SQUARE_GRID_SIZE; col += SQUARE_GRID_SIZE)
        {
            for(auto m = 0; m < BOARD_SIZE; m++)
            {
                auto layout = m_layouts.at(row);
                for(auto n = 0; n < BOARD_SIZE; n++)
                {
                    const auto& block = m_grid_ui.at(m + row).at(n + col);
                    layout ->addItem(block, block -> x(), block -> y());
                }
            }
        }
    }
}
