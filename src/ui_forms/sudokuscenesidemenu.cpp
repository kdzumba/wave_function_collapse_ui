//
// Created by User on 2023/08/14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SudokuSceneSideMenu.h" resolved

#include "sudokuscenesidemenu.h"
#include "ui_SudokuSceneSideMenu.h"
#include "../widgets/Button.h"


SudokuSceneSideMenu::SudokuSceneSideMenu(QWidget *parent) :QWidget(parent), ui(new Ui::SudokuSceneSideMenu)
{
    ui->setupUi(this);
    m_controls_button_group = new ButtonGroup(this);
    setStyleSheet("background: transparent");

    auto* solve_button = new Button("Solve");
    auto* generate_button = new Button("Generate New");
    auto* reset_button = new Button("Reset");
    m_controls_button_group ->add(solve_button);
    m_controls_button_group ->add(generate_button);
    m_controls_button_group ->add(reset_button);
}

SudokuSceneSideMenu::~SudokuSceneSideMenu() {
    delete ui;
}
