//
// Created by User on 2023/08/14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_GraphicsWorkspace.h" resolved

#include "graphicsworkspace.h"
#include "ui_GraphicsWorkspace.h"
#include "sudokuscenesidemenu.h"
#include "../scenes/SudokuScene.h"
#include <QBrush>
#include <QPushButton>

int GraphicsWorkspace::s_current_state_index = 0;

GraphicsWorkspace::GraphicsWorkspace(QWidget *parent) : QWidget(parent), ui(new Ui::GraphicsWorkspace)
{
    ui->setupUi(this);
    init_default_scenes();

    m_graphics_view = new QGraphicsView;
    m_graphics_view ->setScene(m_scene_states.at(0));
    m_main_grid_layout = new QGridLayout(this);
    m_main_grid_layout ->addWidget(m_graphics_view);
}

GraphicsWorkspace::~GraphicsWorkspace()
{
    delete m_graphics_view;
    delete ui;
    delete m_main_grid_layout;
}

void GraphicsWorkspace::init_default_scenes()
{
    //Initialize scenes at the same time as their menus
    auto sudoku_scene = new SudokuScene("puzzles/puzzle11.txt", this);
    m_scene_states.emplace_back(sudoku_scene);
}
