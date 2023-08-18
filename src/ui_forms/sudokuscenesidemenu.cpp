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

    auto* solve_button = new Button("Solve");
    auto* generate_button = new Button("Generate New");
    m_controls_button_group ->add(solve_button);
    m_controls_button_group ->add(generate_button);

    qDebug() << "menu size: " << this -> width() << " , " << this  -> height();
}

SudokuSceneSideMenu::~SudokuSceneSideMenu() {
    delete ui;
}
