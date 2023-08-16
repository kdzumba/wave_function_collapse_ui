//
// Created by User on 2023/08/14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SudokuSceneSideMenu.h" resolved

#include "sudokuscenesidemenu.h"
#include "ui_SudokuSceneSideMenu.h"


SudokuSceneSideMenu::SudokuSceneSideMenu(QWidget *parent) :
        QFrame(parent), ui(new Ui::SudokuSceneSideMenu) {
    ui->setupUi(this);
}

SudokuSceneSideMenu::~SudokuSceneSideMenu() {
    delete ui;
}

void SudokuSceneSideMenu::activate()
{

}
