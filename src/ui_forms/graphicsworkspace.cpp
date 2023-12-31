//
// Created by User on 2023/08/14.
//

// You may need to build the project (run Qt uic code m_generator) to get "ui_GraphicsWorkspace.h" resolved

#include "graphicsworkspace.h"
#include "ui_GraphicsWorkspace.h"
#include "sudokuscenesidemenu.h"
#include "../scenes/SudokuScene.h"
#include "../scenes/ImageGenerationScene.h"
#include <QBrush>
#include <QPushButton>


GraphicsWorkspace::GraphicsWorkspace(QWidget *parent) : QGraphicsView(parent), ui(new Ui::GraphicsWorkspace)
{
    ui->setupUi(this);
    init_default_scenes();
    this ->setScene(m_scene_states.at(1));
    m_main_grid_layout = new QGridLayout;
    this ->setLayout(m_main_grid_layout);
    this ->setRenderHint(QPainter::Antialiasing);
    this->setStyleSheet("background: transparent");

    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_PaintOnScreen);
//    setWindowOpacity(0.5);
}

GraphicsWorkspace::~GraphicsWorkspace()
{
    delete ui;
    delete m_main_grid_layout;
}

void GraphicsWorkspace::init_default_scenes()
{
    //Initialize scenes at the same time as their menus
    auto sudoku_scene = new SudokuScene("puzzles/puzzle1.txt", this);
    auto image_generation_scene = new ImageGenerationScene("images/Summer/", this);

    m_scene_states.emplace_back(sudoku_scene);
    m_scene_states.emplace_back(image_generation_scene);
}
