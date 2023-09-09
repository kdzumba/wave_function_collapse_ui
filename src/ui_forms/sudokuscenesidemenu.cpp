//
// Created by User on 2023/08/14.
//

// You may need to build the project (run Qt uic code m_generator) to get "ui_SudokuSceneSideMenu.h" resolved

#include <QFileDialog>
#include "sudokuscenesidemenu.h"
#include "ui_SudokuSceneSideMenu.h"
#include "../widgets/Button.h"
#include "../widgets/CheckBox.h"
#include "../widgets/Slider.h"


SudokuSceneSideMenu::SudokuSceneSideMenu(QWidget *parent) :AbstractSideMenu(parent), ui(new Ui::SudokuSceneSideMenu)
{
    setStyleSheet("background: rgba(20, 20, 20, 0.1);"
                  "margin: 0 10 0 10;");

    ui->setupUi(this);
    m_controls_button_group = new ButtonGroup();

    auto* solve_button = new Button("Solve");
    QObject::connect(solve_button, SIGNAL(clicked(bool)), this, SLOT(handleSolveButtonClicked()));
    m_controls_button_group ->add(solve_button);

    auto* reset_button = new Button("Reset");
    QObject::connect(reset_button, SIGNAL(clicked(bool)), this, SLOT(handleResetButtonClicked()));
    m_controls_button_group ->add(reset_button);

    auto* generate_button = new Button("Generate New");
    QObject::connect(generate_button, SIGNAL(clicked(bool)), this, SLOT(handleGenerateButtonClicked()));
    m_controls_button_group ->add(generate_button);

    auto* backtrack_select = new CheckBox("Enable Backtracking");
    QObject::connect(backtrack_select, SIGNAL(toggled(bool)), this, SLOT(handleBacktrackToggled(bool)));
    m_controls_button_group -> add(backtrack_select);

    auto* superposition_select = new CheckBox("Show Superpositions");
    QObject::connect(superposition_select, SIGNAL(toggled(bool)), this, SLOT(handleSuperpositionsToggled(bool)));
    m_controls_button_group->add(superposition_select);

    m_controls_slider_group = new SliderGroup();
    auto* backtrack_count_slider = new Slider("Backtrack Count", 0, 5000);
    QObject::connect(backtrack_count_slider, SIGNAL(valueChanged(int)), this, SLOT(handleBacktrackCountChanged(int)));
    m_controls_slider_group->addLayout(backtrack_count_slider->get_layout());

    auto* retries_count_slider = new Slider("Retries Count", 0, 10000);
    QObject::connect(retries_count_slider, SIGNAL(valueChanged(int)), this, SLOT(handleRetriesCountChanged(int)));
    m_controls_slider_group->addLayout(retries_count_slider->get_layout());

    auto controls_layout = new QBoxLayout(QBoxLayout::Direction::TopToBottom, this);
    controls_layout->addLayout(m_controls_button_group->get_layout());
    controls_layout->addLayout(m_controls_slider_group->get_layout());
}

SudokuSceneSideMenu::~SudokuSceneSideMenu() {
    delete ui;
}

void SudokuSceneSideMenu::handleSolveButtonClicked()
{
    emit solveButtonClicked();
}

void SudokuSceneSideMenu::handleResetButtonClicked()
{
    emit resetButtonClicked();
}

void SudokuSceneSideMenu::handleGenerateButtonClicked()
{
    emit generateButtonClicked();
}

void SudokuSceneSideMenu::handleBacktrackToggled(bool isSelected)
{
    emit backtrackOptionSelected(isSelected);
}

void SudokuSceneSideMenu::handleSuperpositionsToggled(bool isSelected)
{
    emit showSuperpositionsSelected(isSelected);
}

void SudokuSceneSideMenu::handleBacktrackCountChanged(int value)
{
    emit backtrackCountChanged(value);
}

void SudokuSceneSideMenu::handleRetriesCountChanged(int value)
{
    emit retriesCountChanged(value);
}