//
// Created by User on 2023/08/14.
//

#include "SudokuScene.h"
#include "../utils.h"
#include "../widgets/Button.h"
#include <QtConcurrent/QtConcurrent>
#include <QGraphicsView>

int SudokuScene::s_animate_count = 0;

SudokuScene::SudokuScene(const std::string& filename, QWidget* parent) : AbstractScene(parent), m_retries_count(0)
{
    m_board = new SudokuBoard(filename);
    m_sudoku_grid = new BoardContainer;
    m_sudoku_menu = new SudokuSceneSideMenu;
    m_animation_thread = nullptr;
    m_reset_thread = nullptr;
    m_retries_count = 1;

    QObject::connect(m_sudoku_menu, SIGNAL(solveButtonClicked()), this, SLOT(animate()));
    QObject::connect(m_sudoku_menu, SIGNAL(resetButtonClicked()), this, SLOT(reset()));
    QObject::connect(m_sudoku_menu, SIGNAL(generateButtonClicked()), this, SLOT(generate()));
    QObject::connect(m_sudoku_menu, SIGNAL(backtrackOptionSelected(bool)), this, SLOT(enableBacktracking(bool)));
    QObject::connect(m_sudoku_menu, SIGNAL(showSuperpositionsSelected(bool)), this, SLOT(showSuperpositions(bool)));
    QObject::connect(m_sudoku_menu, SIGNAL(backtrackCountChanged(int)), this, SLOT(setBacktrackCount(int)));
    QObject::connect(m_sudoku_menu, SIGNAL(retriesCountChanged(int)), this, SLOT(setRetriesCount(int)));
    m_sudoku_menu_proxy = this ->addWidget(m_sudoku_menu);
    m_scene_container = new QGraphicsWidget;

    m_scene_container -> setAttribute(Qt::WA_NoSystemBackground);
    m_scene_container -> setAttribute(Qt::WA_TranslucentBackground);
    m_scene_container -> setAttribute(Qt::WA_PaintOnScreen);
//    setWindowOpacity(0.5);

    m_scene_layout = new QGraphicsLinearLayout(m_scene_container);
    m_scene_layout ->addItem(m_sudoku_grid);
    m_scene_layout ->addItem(m_sudoku_menu_proxy);
    m_scene_layout->setSpacing(10);
    this ->addItem(m_scene_container);

    //Generate a random number between 1 and 180 (QGradient::Present have values from 1 to 180)
    auto rand_gradient = Utils::generate_random_int(1, 180);
    auto gradient = QGradient(QGradient::Preset(rand_gradient));
//    auto gradient = QGradient(QGradient::HealthyWater);
    auto brush = QBrush(gradient);
    this ->setBackgroundBrush(brush);
    init();
}

void SudokuScene::animate()
{
    auto solve = [&]() -> void{
        //This guy just collapses the first block to kick star generate.
        //Need it in here after reset because the first solves removes
        //all available options (fully solved)
        reset();
        m_board -> init_solve();
        auto retries_count = 0;
        while(!(m_board -> is_fully_solved()) && retries_count < m_retries_count)
        {
            reset();
            m_board -> solve();
            retries_count++;
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
    if(m_animation_thread != nullptr && m_animation_thread->isRunning())
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

void SudokuScene::enableBacktracking(bool isBacktrackingEnabled)
{
    m_board -> setBacktrackingEnabled(isBacktrackingEnabled);
}

void SudokuScene::showSuperpositions(bool shouldShowSuperpositions)
{
    m_board -> setShowSuperpositions(shouldShowSuperpositions);
}

void SudokuScene::setBacktrackCount(int count)
{
    m_board -> setBacktrackCount(count);
}

void SudokuScene::setRetriesCount(int count)
{
    this -> m_retries_count = count;
}