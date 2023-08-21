//
// Created by User on 2023/08/14.
//

#include "SudokuScene.h"
#include "../utils.h"
#include "../widgets/Button.h"
#include <QtConcurrent/QtConcurrent>

int SudokuScene::s_animate_count = 0;

SudokuScene::SudokuScene(const std::string& filename, QWidget* parent) : AbstractScene(parent), m_retries_count(0)
{
    m_board = new SudokuBoard(filename);
    m_sudoku_grid = new BoardContainer;
    m_sudoku_menu = new SudokuSceneSideMenu;


    QObject::connect(m_sudoku_menu, SIGNAL(solveButtonClicked()), this, SLOT(animate()));
    QObject::connect(m_sudoku_menu, SIGNAL(resetButtonClicked()), this, SLOT(reset()));
    QObject::connect(m_sudoku_menu, SIGNAL(generateButtonClicked()), this, SLOT(generate()));

    m_sudoku_menu_proxy = this ->addWidget(m_sudoku_menu);
    m_scene_container = new QGraphicsWidget;
    m_scene_layout = new QGraphicsGridLayout(m_scene_container);

    m_scene_layout ->addItem(m_sudoku_grid, 0,0);
    m_scene_layout ->addItem(m_sudoku_menu_proxy, 1, 0);
    this ->addItem(m_scene_container);

    //Generate a random number between 1 and 180 (QGradient::Present have values from 1 to 180)
    auto rand_gradient = Utils::generate_random_int(1, 180);
    auto gradient = QGradient(QGradient::Preset(rand_gradient));
//    auto gradient = QGradient(QGradient::GentleCare);
    auto brush = QBrush(gradient);
    this ->setBackgroundBrush(brush);
    init();
}

void SudokuScene::animate()
{
    auto solve = [&]() -> void{
        //This guy just collapses the first block to kick star solve.
        //Need it in here after reset because the first solves removes
        //all available options (fully solved)
        reset();
        m_board -> init_solve();
        m_retries_count = 0;
        while(!(m_board -> is_fully_solved()) && m_retries_count < 10000)
        {
            reset();
            m_board -> solve();
            m_retries_count++;
        }
    };

    m_animation_thread = QThread::create(solve);
    m_animation_thread ->start();
    s_animate_count++;
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
            const auto& block_model = initial_board.at(row).at(col);
            auto tile = new GridTile(block_model.get(), x, y);
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

            grid_col_index++;
            grid_layout_index++;
        }
        grid_col_index = 0;
        grid_row_index++;
    }
}

void SudokuScene::reset()
{
    auto reset = [&]() -> void{
        m_board -> reset();
    };
    m_reset_thread = QThread::create(reset);
    m_reset_thread->start();
    m_reset_thread -> wait();
}

void SudokuScene::generate()
{
    m_animation_thread -> quit();
    qDebug() << "Before generating: " << this -> items().size();

    //Remove all items from m_grid_ui (messes up the indexing in arrangeItems()
    for(const auto& row : m_grid_ui)
    {
        for(auto tile : row)
        {
            this ->removeItem(tile);
            delete tile;
        }
    }
    m_grid_ui.clear();

    //No need for this board anymore, free up memory and reset to nullptr (avoid gibberish)
    delete m_board;
    m_board = nullptr;

    m_sudoku_grid = nullptr;

    auto file_index = Utils::generate_random_int(1, 11);
    auto filename = std::string("puzzles/puzzle") + std::to_string(file_index) + ".txt";

    m_board = new SudokuBoard(filename);
    m_sudoku_grid = new BoardContainer;
    init();
    qDebug() << "After generating: " << this -> items().size();
}
