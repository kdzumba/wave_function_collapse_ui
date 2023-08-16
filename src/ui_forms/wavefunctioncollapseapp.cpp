//
// Created by User on 2023/08/14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_WaveFunctionCollapseApp.h" resolved

#include "wavefunctioncollapseapp.h"
#include "ui_WaveFunctionCollapseApp.h"
#include "sudokuscenesidemenu.h"
#include <QLayout>

WaveFunctionCollapseApp::WaveFunctionCollapseApp(QWidget *parent) : QMainWindow(parent),
ui(new Ui::WaveFunctionCollapseApp)
{
    ui->setupUi(this);
    m_workspace = new GraphicsWorkspace(this);
    this ->resize(1920, 800);
    this ->setWindowTitle("Wave Function Collapse");
}

WaveFunctionCollapseApp::~WaveFunctionCollapseApp()
{
    delete ui;
    delete m_workspace;
}

void WaveFunctionCollapseApp::run()
{
    this -> show();
}
