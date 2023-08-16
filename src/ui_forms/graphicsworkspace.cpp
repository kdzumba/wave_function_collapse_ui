//
// Created by User on 2023/08/14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_GraphicsWorkspace.h" resolved

#include "graphicsworkspace.h"
#include "ui_GraphicsWorkspace.h"
#include "sudokuscenesidemenu.h"
#include "../scenes/SudokuScene.h"
#include <QBrush>

int GraphicsWorkspace::s_current_state_index = 0;

GraphicsWorkspace::GraphicsWorkspace(QWidget *parent) : QWidget(parent), ui(new Ui::GraphicsWorkspace)
{
    //Create all the scenes that can be activated on this workspace
    init_default_scenes();

    //Initialize the graphics view and set its scene to the first scene state
    m_view = new QGraphicsView(this);
    m_view ->setScene(m_scene_states.at(0));

    ui->setupUi(this);
    m_view -> show();
}

GraphicsWorkspace::~GraphicsWorkspace()
{
    delete m_view;
    delete ui;
}

void GraphicsWorkspace::init_default_scenes()
{
    //Initialize scenes at the same time as their menus
    auto sudoku_scene = new SudokuScene("puzzles/puzzle9.txt", this);
    m_scene_states.emplace_back(sudoku_scene);

    auto sudokuMenu = new SudokuSceneSideMenu(this);
    m_menus.emplace_back(sudokuMenu);
    //TODO: Add the sudoku scene
}
